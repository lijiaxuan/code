# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'test.ui'
#
# Created: Wed Jan 06 19:09:24 2016
#      by: PyQt4 UI code generator 4.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName(_fromUtf8("Dialog"))
        Dialog.resize(598, 389)
        self.data = QtGui.QListWidget(Dialog)
        self.data.setGeometry(QtCore.QRect(220, 20, 341, 261))
        self.data.setObjectName(_fromUtf8("data"))
        self.groupBox = QtGui.QGroupBox(Dialog)
        self.groupBox.setGeometry(QtCore.QRect(20, 20, 191, 221))
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.commdir1 = QtGui.QComboBox(self.groupBox)
        self.commdir1.setGeometry(QtCore.QRect(80, 20, 71, 22))
        self.commdir1.setObjectName(_fromUtf8("commdir1"))
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setGeometry(QtCore.QRect(10, 26, 54, 12))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(self.groupBox)
        self.label_2.setGeometry(QtCore.QRect(10, 110, 54, 12))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.opencomm = QtGui.QPushButton(self.groupBox)
        self.opencomm.setGeometry(QtCore.QRect(10, 160, 75, 23))
        self.opencomm.setObjectName(_fromUtf8("opencomm"))
        self.baud = QtGui.QComboBox(self.groupBox)
        self.baud.setGeometry(QtCore.QRect(80, 100, 71, 22))
        self.baud.setObjectName(_fromUtf8("baud"))
        self.baud.addItem(_fromUtf8(""))
        self.closecomm = QtGui.QPushButton(self.groupBox)
        self.closecomm.setGeometry(QtCore.QRect(100, 160, 75, 23))
        self.closecomm.setObjectName(_fromUtf8("closecomm"))
        self.label_3 = QtGui.QLabel(self.groupBox)
        self.label_3.setGeometry(QtCore.QRect(10, 66, 54, 12))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.commdir2 = QtGui.QComboBox(self.groupBox)
        self.commdir2.setGeometry(QtCore.QRect(80, 60, 71, 22))
        self.commdir2.setObjectName(_fromUtf8("commdir2"))
        self.is_save = QtGui.QCheckBox(Dialog)
        self.is_save.setGeometry(QtCore.QRect(220, 350, 121, 16))
        self.is_save.setObjectName(_fromUtf8("is_save"))
        self.filepath_edit = QtGui.QLineEdit(Dialog)
        self.filepath_edit.setGeometry(QtCore.QRect(220, 320, 301, 20))
        self.filepath_edit.setReadOnly(True)
        self.filepath_edit.setObjectName(_fromUtf8("filepath_edit"))
        self.path = QtGui.QPushButton(Dialog)
        self.path.setGeometry(QtCore.QRect(530, 320, 41, 23))
        self.path.setObjectName(_fromUtf8("path"))

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(_translate("Dialog", "Dialog", None))
        self.groupBox.setTitle(_translate("Dialog", "comm", None))
        self.label.setText(_translate("Dialog", "com1", None))
        self.label_2.setText(_translate("Dialog", "baud", None))
        self.opencomm.setText(_translate("Dialog", "open", None))
        self.baud.setItemText(0, _translate("Dialog", "115200", None))
        self.closecomm.setText(_translate("Dialog", "close", None))
        self.label_3.setText(_translate("Dialog", "com2", None))
        self.is_save.setText(_translate("Dialog", "save to file", None))
        self.path.setText(_translate("Dialog", "...", None))

