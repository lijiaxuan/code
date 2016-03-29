# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'test.ui'
#
# Created: Sat Jan 23 11:45:58 2016
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
        self.data.setGeometry(QtCore.QRect(220, 20, 341, 141))
        self.data.setObjectName(_fromUtf8("data"))
        self.groupBox = QtGui.QGroupBox(Dialog)
        self.groupBox.setGeometry(QtCore.QRect(20, 20, 191, 141))
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.comboBox = QtGui.QComboBox(self.groupBox)
        self.comboBox.setGeometry(QtCore.QRect(80, 20, 71, 22))
        self.comboBox.setObjectName(_fromUtf8("comboBox"))
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setGeometry(QtCore.QRect(10, 26, 54, 12))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(self.groupBox)
        self.label_2.setGeometry(QtCore.QRect(10, 60, 54, 12))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.opencomm = QtGui.QPushButton(self.groupBox)
        self.opencomm.setGeometry(QtCore.QRect(10, 100, 75, 23))
        self.opencomm.setObjectName(_fromUtf8("opencomm"))
        self.baud = QtGui.QComboBox(self.groupBox)
        self.baud.setGeometry(QtCore.QRect(80, 60, 71, 22))
        self.baud.setObjectName(_fromUtf8("baud"))
        self.baud.addItem(_fromUtf8(""))
        self.baud.addItem(_fromUtf8(""))
        self.closecomm = QtGui.QPushButton(self.groupBox)
        self.closecomm.setGeometry(QtCore.QRect(100, 100, 75, 23))
        self.closecomm.setObjectName(_fromUtf8("closecomm"))
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
        self.groupBox_2 = QtGui.QGroupBox(Dialog)
        self.groupBox_2.setGeometry(QtCore.QRect(20, 180, 191, 151))
        self.groupBox_2.setObjectName(_fromUtf8("groupBox_2"))
        self.spliter_edit = QtGui.QLineEdit(self.groupBox_2)
        self.spliter_edit.setGeometry(QtCore.QRect(80, 27, 91, 20))
        self.spliter_edit.setObjectName(_fromUtf8("spliter_edit"))
        self.label_3 = QtGui.QLabel(self.groupBox_2)
        self.label_3.setGeometry(QtCore.QRect(20, 30, 54, 12))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.framele_edit = QtGui.QLineEdit(self.groupBox_2)
        self.framele_edit.setGeometry(QtCore.QRect(80, 70, 91, 20))
        self.framele_edit.setObjectName(_fromUtf8("framele_edit"))
        self.label_4 = QtGui.QLabel(self.groupBox_2)
        self.label_4.setGeometry(QtCore.QRect(20, 70, 54, 12))
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.format_edit = QtGui.QLineEdit(self.groupBox_2)
        self.format_edit.setGeometry(QtCore.QRect(80, 110, 91, 20))
        self.format_edit.setObjectName(_fromUtf8("format_edit"))
        self.label_5 = QtGui.QLabel(self.groupBox_2)
        self.label_5.setGeometry(QtCore.QRect(20, 112, 54, 12))
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.x = QtGui.QGroupBox(Dialog)
        self.x.setGeometry(QtCore.QRect(220, 180, 341, 131))
        self.x.setObjectName(_fromUtf8("x"))
        self.label_6 = QtGui.QLabel(self.x)
        self.label_6.setGeometry(QtCore.QRect(20, 30, 54, 12))
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.label_7 = QtGui.QLabel(self.x)
        self.label_7.setGeometry(QtCore.QRect(20, 60, 54, 12))
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.label_8 = QtGui.QLabel(self.x)
        self.label_8.setGeometry(QtCore.QRect(20, 90, 54, 12))
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.m_x = QtGui.QLabel(self.x)
        self.m_x.setGeometry(QtCore.QRect(110, 30, 54, 12))
        self.m_x.setObjectName(_fromUtf8("m_x"))
        self.m_y = QtGui.QLabel(self.x)
        self.m_y.setGeometry(QtCore.QRect(110, 60, 54, 12))
        self.m_y.setObjectName(_fromUtf8("m_y"))
        self.m_angle = QtGui.QLabel(self.x)
        self.m_angle.setGeometry(QtCore.QRect(110, 90, 54, 12))
        self.m_angle.setObjectName(_fromUtf8("m_angle"))

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(_translate("Dialog", "Dialog", None))
        self.groupBox.setTitle(_translate("Dialog", "comm", None))
        self.label.setText(_translate("Dialog", "com", None))
        self.label_2.setText(_translate("Dialog", "baud", None))
        self.opencomm.setText(_translate("Dialog", "open", None))
        self.baud.setItemText(0, _translate("Dialog", "9600", None))
        self.baud.setItemText(1, _translate("Dialog", "115200", None))
        self.closecomm.setText(_translate("Dialog", "close", None))
        self.is_save.setText(_translate("Dialog", "save to file", None))
        self.path.setText(_translate("Dialog", "...", None))
        self.groupBox_2.setTitle(_translate("Dialog", "spliter(hex)", None))
        self.label_3.setText(_translate("Dialog", "spliter", None))
        self.label_4.setText(_translate("Dialog", "framelen", None))
        self.label_5.setText(_translate("Dialog", "format", None))
        self.x.setTitle(_translate("Dialog", "GroupBox", None))
        self.label_6.setText(_translate("Dialog", "x:", None))
        self.label_7.setText(_translate("Dialog", "y:", None))
        self.label_8.setText(_translate("Dialog", "angle:", None))
        self.m_x.setText(_translate("Dialog", "0.0", None))
        self.m_y.setText(_translate("Dialog", "0.0", None))
        self.m_angle.setText(_translate("Dialog", "0.0", None))

