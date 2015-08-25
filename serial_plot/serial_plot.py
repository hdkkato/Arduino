# -*- coding: utf-8 -*-
import sys, time, serial
import numpy as np
from numpy import fft
from PySide import QtGui, QtCore

# use PySide as matplotlib backend explictly
import matplotlib
matplotlib.use('Qt4Agg')
matplotlib.rcParams['backend.qt4'] = 'PySide'

import matplotlib.pyplot as plt
plt.style.use('ggplot')

# link matplotlib to pyside
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt4agg import (
    FigureCanvasQTAgg as FigureCanvas,
    NavigationToolbar2QT as NavigationToolbar)

from serial_plot_ui import Ui_MainWindow
from PySide.QtGui import QMainWindow


def serial_ports():
    """Lists serial ports

    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
        A list of available serial ports
    """
    import sys
    import glob
    import serial
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def sig_fft(sig, duration):
    """
    execute fft
    return the result of fft as list
    """
    sig = np.array(sig)
    sig = sig - sig.mean()

    # execute fft
    sig_fft = fft.fft(sig)
    freq = fft.fftfreq(len(sig), d=duration)

    # fold result as fft returns symmetric spectrum
    sig_fft = sig_fft[:(len(sig_fft)/2)]
    sig_fft = abs(sig_fft)
    freq = freq[:(len(freq)/2)]
    return freq.tolist(), sig_fft.tolist()


class SerialReader(QtCore.QObject):
    """
    SerialReader class is running apart from MainWindodw
    """

    sig_status = QtCore.Signal(list)
    sig_termino = QtCore.Signal()

    def __init__(self):
        super(SerialReader, self).__init__()
        self.stopped = False
        self.mutex = QtCore.QMutex()

    def stop(self):
        with QtCore.QMutexLocker(self.mutex):
            self.stopped = True
        self.sig_termino.emit()

    def finish_read(self, val):
        self.serial_close()
        val = val.replace(b'\x03', b'').decode()  # remove control code
        data = [int(item) for item in val.split()]
        self.sig_status.emit(data)

    def run(self):
        self.stopped = False
        self.serial_open()
        self.serial_read()
        self.stop()

    def serial_open(self):
        if hasattr(self, 'com') and self.com.isOpen():
            self.com.flushInput()
            return
        port = serial_ports()
        self.com = serial.Serial(port=port[0], baudrate=115200)
        self.com.flushInput()
        time.sleep(1)
        self.com.write(self.mode)
        time.sleep(1)

    def serial_close(self):
        if not hasattr(self, 'com'):
            return
        if not self.com.isOpen():
            return
        self.com.write(b'\x06')  # ACK (ACKnowledge)
        time.sleep(1)
        self.com.flushInput()
        self.com.close()

    def serial_read(self):
        val = bytearray(b'')
        while True:
            # forcibly execute event in busy loop
            QtGui.QApplication.processEvents()
            if self.stopped:
                self.serial_close
                break
            if self.com.readable():
                # load all character in serial buffer
                ch = self.com.read(self.com.inWaiting())
                val += ch
                if b'\x03' in ch:  # ETX (End of TeXt)
                    self.finish_read(val)
                    break


class MainWindodw(QMainWindow, Ui_MainWindow):
    """main window of application"""
    def __init__(self):
        super(MainWindodw, self).__init__()
        self.setupUi(self)
        self.isTrigMode = False
        self.isRunMode = False
        self.button_list = [self.run_radio_button,
                            self.trig_radio_button,
                            self.run_push_button]
        self.data = []
        self.freq = []
        self.res = []

        # implement button's function
        self.trig_radio_button.clicked.connect(self.trig_toggled)
        self.run_radio_button.clicked.connect(self.run_toggled)
        self.run_push_button.clicked.connect(self.run_pushed)
        self.stop_push_button.clicked.connect(self.stop_pushed)
        self.saveraw_push_button.clicked.connect(self.saveraw_pushed)
        self.savefft_push_button.clicked.connect(self.savefft_pushed)

        # add empty plot
        fig1 = Figure()
        axfig1 = fig1.add_subplot(111)
        axfig1.plot()
        self.add_rawmpl(fig1)

        fig2 = Figure()
        axfig2 = fig2.add_subplot(111)
        axfig2.plot()
        self.add_fftmpl(fig2)

        thread = QtCore.QThread()
        self.reader = SerialReader()
        self.reader.moveToThread(thread)
        self.reader.sig_status.connect(self.serial_plot)
        self.reader.sig_termino.connect(self.enable_button)

    def add_rawmpl(self, fig):
        self.raw_canvas = FigureCanvas(fig)
        self.vl_raw.addWidget(self.raw_canvas)
        self.raw_canvas.draw()
        self.raw_toolbar = NavigationToolbar(self.raw_canvas,
            self, coordinates=True)
        self.vl_raw.addWidget(self.raw_toolbar)

    def add_fftmpl(self, fig):
        self.fft_canvas = FigureCanvas(fig)
        self.vl_fft.addWidget(self.fft_canvas)
        self.fft_canvas.draw()
        self.fft_toolbar = NavigationToolbar(self.fft_canvas,
            self, coordinates=True)
        self.vl_fft.addWidget(self.fft_toolbar)

    def remove_rawmpl(self):
        self.vl_raw.removeWidget(self.raw_canvas)
        self.raw_canvas.close()
        self.vl_raw.removeWidget(self.raw_toolbar)
        self.fft_toolbar.close()

    def remove_fftmpl(self):
        self.vl_fft.removeWidget(self.fft_canvas)
        self.fft_canvas.close()
        self.vl_fft.removeWidget(self.fft_toolbar)
        self.fft_toolbar.close()

    def trig_toggled(self):
        print('trig is toggled')
        self.isTrigMode = True
        self.isRunMode = False

    def run_toggled(self):
        print('run is toggled')
        self.isTrigMode = False
        self.isRunMode = True

    def run_pushed(self):
        if self.isTrigMode: self.reader.mode = b't'
        elif self.isRunMode: self.reader.mode = b'r'
        else:
            self.errorDialog('Select Mode')
            return
        self.disable_button()
        self.reader.run()
        time.sleep(5)

    def stop_pushed(self):
        self.reader.stop()
        self.reader.serial_close()

    def saveraw_pushed(self):
        if not self.data:
            self.errorDialog('No measurement')
            return
        filepath = QtGui.QFileDialog.getSaveFileName(filter='*.dat')[0]
        with open(filepath, 'w') as f:
            for item in self.data:
                f.write(str(item) + '\n')

    def savefft_pushed(self):
        if not self.freq:
            self.errorDialog('No measurement')
            return
        filepath = QtGui.QFileDialog.getSaveFileName(filter='*.dat')[0]
        with open(filepath, 'w') as f:
            for fre, res in zip(self.freq, self.res):
                f.write(str(fre) + ' ' + str(res) + '\n')

    def serial_plot(self, data):
        self.remove_rawmpl()
        self.remove_fftmpl()
        self.data = data
        self.freq, self.res = sig_fft(data, 1.0/20000)
        fig1 = Figure()
        axfig1 = fig1.add_subplot(111)
        axfig1.plot(self.data)
        fig2 = Figure()
        axfig2 = fig2.add_subplot(111)
        axfig2.plot(self.freq, self.res)
        self.add_rawmpl(fig1)
        self.add_fftmpl(fig2)

    def enable_button(self):
        for btn in self.button_list:
            btn.setEnabled(True)

    def disable_button(self):
        for btn in self.button_list:
            btn.setEnabled(False)

    def errorDialog(self, message):
        e = QtGui.QErrorMessage()
        e.showMessage(message)
        e.exec_()


def main():
    """
    entry point for this program
    """
    app = QtGui.QApplication(sys.argv)
    window = MainWindodw()
    window.show()
    app.exec_()


if __name__ == '__main__':
    main()