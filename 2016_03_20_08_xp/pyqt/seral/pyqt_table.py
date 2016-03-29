
from PyQt4.QtGui import *  
from PyQt4.QtCore import *  
import sys  
from test import *
import time
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
            i = i + 1
            time.sleep(1)
            self.window.update()

class TestDialog(QDialog,Ui_Dialog):
    def __init__(self,parent=None):  
        super(TestDialog,self).__init__(parent)  
        self.setupUi(self)
        self.available_port = []
        self.get_available_port()
        self.connect(self.open,SIGNAL('clicked()'),self.open_comm)
        self.table.setColumnWidth(0,90)
        self.table.setColumnWidth(1,60)
        self.table.setColumnWidth(2,80)
    def get_available_port(self):
        for i in range(10):
            try:
                s = serial.Serial(i)
                self.available_port.append(i)
                s.close()
            except Exception:
                pass
        for i in range(len(self.available_port)):
            self.comboBox.addItem(_fromUtf8(""))
            self.comboBox.setItemText(i,'COM'+str(self.available_port[i]+1))
    def open_comm(self):
        self.data.addItem('ss')
        num = [2,3,4]
        count = self.table.rowCount()
        print count
        self.table.setRowCount(3)
        for i in range(len(num)):
            newItem = QTableWidgetItem(str(num[i]))
            self.table.setItem((count - 1) / 3,i,newItem)
        print "open"
    def update(self):
        pass
app=QApplication(sys.argv)

dialog=TestDialog()

work = worker(dialog)
work.start()
dialog.show()
app.exec_()  