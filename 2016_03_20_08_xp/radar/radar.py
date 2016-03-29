
from PyQt4.QtGui import *  
from PyQt4.QtCore import *  
import sys  
from test import *
import time
import os
import binascii
basedir = os.getcwd()
print basedir
import struct

import serial

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s
i = 0

class worker(QThread):
    def __init__(self,window):
        QThread.__init__(self,None)
        self.window = window
    def run(self):
        global i
        while 1:
            num2read1 = 0
            num2read2 = 0
            data = ''
            time.sleep(0.01)
            if self.window.is_open == True:
                num2read1 =  self.window.comm_sft.inWaiting()
                if num2read1 != 0:
                    data = self.window.comm_sft.read(num2read1)
                    if len(data) > 0:
                        self.window.update(data,0)
                num2read2 =  self.window.comm_dev.inWaiting()
                if num2read2 != 0:
                    data = self.window.comm_dev.read(num2read2)
                    if len(data) > 0:
                        self.window.update(data,1)

class TestDialog(QDialog,Ui_Dialog):
    def __init__(self,parent=None):  
        super(TestDialog,self).__init__(parent)  
        self.setupUi(self)
        self.available_port = []
        self.get_available_port()
        self.is_open = False
        self.is_save_to_file = False
        self.comm_sft = None
        self.closecomm.setDisabled(True)
        self.connect(self.opencomm,SIGNAL('clicked()'),self.open_comm)
        self.connect(self.closecomm,SIGNAL('clicked()'),self.close_comm)
        self.connect(self.path,SIGNAL('clicked()'),self.get_path)
        self.content = ''

        self.filepath_edit.setText(basedir+'\data')
        self.filepath = basedir+'\data'
        self.rawdatafile = open(basedir + '\\rawdata','w')
        self.logfile = None
    def get_available_port(self):
        for i in range(10):
            try:
                s = serial.Serial(i)
                self.available_port.append(i)
                s.close()
            except Exception:
                pass
        for i in range(len(self.available_port)):
            self.commsft.addItem(_fromUtf8(""))
            self.commsft.setItemText(i,'COM'+str(self.available_port[i]+1))
            self.commdev.addItem(_fromUtf8(""))
            self.commdev.setItemText(i,'COM'+str(self.available_port[i]+1))
    def open_comm(self):
        try:

            try:
                self.filepath = str(self.filepath_edit.text())
                self.logfile = open(self.filepath,'w')
            except Exception:
                QMessageBox.warning(self,'warning','fail to open file')
                return

            comm_num = self.commsft.currentIndex()
            self.comm_sft = serial.Serial(self.available_port[comm_num],baudrate=115200)
            comm_num = self.commdev.currentIndex()
            self.comm_dev = serial.Serial(self.available_port[comm_num],baudrate=115200)
            self.is_open = True
            self.opencomm.setDisabled(True)
            self.commsft.setDisabled(True)
            self.commdev.setDisabled(True)
            self.baud.setDisabled(True)
            self.closecomm.setDisabled(False)

        except:
            print 'open error'
        print "open"
    def close_comm(self):
        try:
            self.comm_sft.close()
            self.comm_dev.close()
            self.is_open = False
            self.opencomm.setDisabled(False)
            self.commsft.setDisabled(False)
            self.commdev.setDisabled(False)
            self.baud.setDisabled(False)
            self.closecomm.setDisabled(True)
            self.logfile.close()
            self.rawdatafile.close()
        except:
            print 'close error'
    def get_path(self):
        s=QFileDialog.getExistingDirectory(self,"Open file dialog",basedir)
        print s
        self.filepath_edit.setText(str(s).replace('/','\\')+'\data')

    def update(self,data,dir):
        if dir == 1:
            self.rawdatafile.write('up' + data + '\r\n')
            self.comm_sft.write(data)
        elif dir == 0:
            self.rawdatafile.write('down'+ data + '\r\n')
            self.comm_dev.write(data)
        # self.content = self.content + data
        # if len(self.content) <= self.framelenth:
        #     return
        # frm = self.content.split(self.spliter)
        # for i in range(len(frm)):
        #     if len(frm[i]) == self.framelenth - len(self.spliter):
        #         self.display(frm[i])
        #     elif len(frm[i]) <= self.framelenth:
        #         continue
        #
        # pass
    def display(self,data):
        pass
        # dip = struct.unpack(self.fmt,data)
        # self.logfile.write(str(dip)+'\r\n')
app=QApplication(sys.argv)

dialog=TestDialog()

work = worker(dialog)
work.start()
dialog.show()
app.exec_()  