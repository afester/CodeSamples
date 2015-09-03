'''
Created on 01.09.2015

@author: afester
'''
# 
from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, pyqtSignal, Qt, QRectF, QPointF, QDate, QTime
from PyQt5.QtWidgets import QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QMessageBox, QListView
from PyQt5.QtGui import QStandardItem, QStandardItemModel, QIcon, QPalette, QPainter, QImage, QBrush
from PyQt5.QtSvg import QSvgRenderer
import math
import sys, os, platform, re, sqlite3, logging

from ui.Data import Ui_Form

# import pkg_resources, data
from os.path import expanduser
# 
# from ui.EditorWidget import EditorWidget
# from ui.BrowserWidget import BrowserWidget
# from model.Page import LocalPage
# from ui.SearchWidget import Ui_SearchWidget
# 
from Settings import Settings
from PyQt5.Qt import QPoint, QSizeF

class ShadowWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.texture = QImage("ui/gras.png")
        self.svgRenderer = QSvgRenderer("ui/tree.svg")

        # set the gras texture as the background image
        self.setAutoFillBackground(True)
        pal = self.palette()
        pal.setBrush(QPalette.Window, QBrush(self.texture))
        self.setPalette(pal)
        self.angle = 0

    def setAngle(self, value):
        self.angle = value
        self.repaint()

    def paintEvent(self, event):
        radius = 50
        
        treeSize = QSizeF(2 * radius, 2 * radius)
        bounds = QRectF( (self.width() - treeSize.height()) / 2,
                         (self.height() - treeSize.width()) / 2,
                         treeSize.width(),
                         treeSize.height())

        painter = QPainter(self)

        # calculate relative coordinates for the shadow center (right on the 
        # arc of a circle around the tree) 
        b = math.radians(self.angle)
        x = math.sin(b) * radius
        y = math.cos(b) * radius

        # draw the shadow
        center = QPointF((self.width() - treeSize.height()) / 2,
                         (self.height() - treeSize.width()) / 2)
        shadow = QRectF(center.x() - x, center.y() - y, 100, 100)
        painter.setBrush(Qt.black)
        painter.setPen(Qt.black)
        painter.setOpacity(0.5)
        painter.drawEllipse(shadow)

        # draw the tree
        painter.setOpacity(1)
        self.svgRenderer.render(painter, bounds)


class DataWidget(QWidget):
    
    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.ui = Ui_Form()
        self.ui.setupUi(self)



class MainWindow(QMainWindow):
# 
    l = logging.getLogger('MainWindow')
# 
    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.l.debug('Initializing MainWindow ...')

        self.setWindowTitle('Shadow')
#         app.setWindowIcon(QIcon(':/icons/mynpad.png'))
# 
#         if os.name == 'nt':
#             # On Windows, make sure to use a unique Application User Model Id, otherwise
#             # Windows shows the default python icon in the taskbar
#             # see http://stackoverflow.com/questions/1551605/how-to-set-applications-taskbar-icon-in-windows-7
#             myappid = 'afester.mynpad'
#             import ctypes; ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
 
        self.theApplication = app
        app.aboutToQuit.connect(self.saveState)
 
        # read the local configuration file
        iniPath = 'shadow.ini'
        if not os.path.exists(iniPath):
            iniPath = os.path.join(expanduser("~"), iniPath)
        self.settings = Settings(iniPath)
        self.settings.load()

        # Set up the menu bar
        menuBar = QMenuBar(self)

        exitAction = QAction("Exit", self, triggered=self.theApplication.exit)
        fileMenu = menuBar.addMenu("&File")
        fileMenu.addAction(exitAction)
 
        aboutAction = QAction("About ...", self, triggered = self.handleAbout)
        helpMenu = menuBar.addMenu("&Help")
        helpMenu.addAction(aboutAction)

        self.setMenuBar(menuBar)
 
#         # Setup the status bar
#         self.statusBar = QStatusBar()
#         self.statusBar.showMessage("Ready.")
#         self.setStatusBar(self.statusBar)
#         self.mainWidget = CentralWidget(self, self.settings)
#         self.mainWidget.updateWindowTitle.connect(self.updateWindowTitle)
#         self.setCentralWidget(self.mainWidget)

        self.centralWidget = QWidget()
        self.left = DataWidget(self.centralWidget)
        self.right = ShadowWidget(self.centralWidget)

        self.left.ui.dateEdit.dateChanged.connect(self.updateDate)
        self.left.ui.timeEdit.timeChanged.connect(self.updateTime)

        layout = QHBoxLayout()
        self.centralWidget.setLayout(layout)
        layout.addWidget(self.left)
        layout.addWidget(self.right)
        layout.setStretch(0, 0);
        layout.setStretch(1, 1);

        self.setCentralWidget(self.centralWidget)

        # Reset main window size and position
        pos = self.settings.getMainWindowPos()
        self.move(pos.x(), pos.y())
        size = self.settings.getMainWindowSize()
        self.resize(size)

        self.time = QTime()
        self.date = QDate()

        # sample values
        self.left.ui.latitudeLineEdit.setValue(48.1)
        self.left.ui.LongitudeLineEdit.setValue(11.6)
        self.left.ui.treeHeightLineEdit.setValue(10)
        self.left.ui.dateEdit.setDate(QDate(2006, 8, 6))
        self.left.ui.timeEdit.setTime(QTime(6, 0))  # UTC

    def updateDate(self, date):
        self.date = date
        self.recalculate()

    def updateTime(self, time):
        self.time = time
        self.recalculate()


    def getJulianDay(self):
        #print("Date: {}".format(self.date))
        #print("Time: {}".format(self.time))
        JD = float(self.date.toJulianDay())
        minuteOfDay = self.time.msecsSinceStartOfDay() / (60 * 1000)
        timeFraction = minuteOfDay / (24 * 60) - 0.5    # time base on 01/01/2000 is 12:00 !!!!
        #print("minuteOfDay: {}".format(minuteOfDay))
        #print("timeFraction: {}".format(timeFraction))
        JD = JD + timeFraction
        return JD 

    def recalculate(self):

        JD = self.getJulianDay()
        self.left.ui.jDLineEdit.setText(str(JD))

        n = JD - 2451545.0
        self.left.ui.timeVariableNLineEdit.setText(str(n))

        L = 280.460 + 0.9856474 * n
        divis = int(L / 360)
        L = L - divis * 360
        self.left.ui.LVariableLineEdit.setText(str(L))

        g = 357.528 +0.9856003 * n
        divis = int(g / 360)
        g = g - divis * 360
        self.left.ui.meanAnomalyGLineEdit.setText(str(g))

        Lambda = L + 1.915 * math.sin(math.radians(g)) + 0.01997 * math.sin(math.radians(2 * g)) 
        self.left.ui.eclipticLengthLineEdit.setText(str(Lambda))

        epsilon = 23.439 -0.0000004 * n
        self.left.ui.eclipticSkewLineEdit.setText(str(epsilon))

        denominator = math.cos(math.radians(Lambda))
        alpha = math.atan( math.cos(math.radians(epsilon)) * math.sin(math.radians(Lambda)) / denominator )
        if denominator < 0:
            alpha = alpha + math.radians(180)
        alpha = math.degrees(alpha)
        self.left.ui.rightAscensionLineEdit.setText(str(alpha))

        delta = math.asin(math.sin(math.radians(epsilon)) * math.sin(math.radians(Lambda)))
        delta = math.degrees(delta)
        self.left.ui.declinationLineEdit.setText(str(delta))

        JD_0 = float(self.date.toJulianDay()) - 0.5   # time base on 01/01/2000 is 12:00 !!!!
        self.left.ui.jD_0LineEdit.setText(str(JD_0))
        
        T_0 = (JD_0 - 2451545.0) / 36525
        self.left.ui.t_0LineEdit.setText(str(T_0))

        T = self.time.msecsSinceStartOfDay() / (60 * 60 * 1000)
        ThetaH_G = 6.697376 + 2400.05134 * T_0 + 1.002738 * T
        divis = int(ThetaH_G / 24)
        ThetaH_G = ThetaH_G - divis * 24

        self.left.ui.sternzeitH_GLineEdit.setText(str(ThetaH_G))

        Theta_G = ThetaH_G * 15
        self.left.ui.greenwichStundenwinkelFP_GLineEdit.setText(str(Theta_G))

        longitude = self.left.ui.LongitudeLineEdit.value()  # lambda
        Theta = Theta_G + longitude
        divis = int(Theta / 360)
        Theta = Theta - divis * 360

        self.left.ui.stundenwinkelFrHlingspunktLineEdit.setText(str(Theta))

        tau = Theta - alpha
        self.left.ui.stundenwinkelLineEdit.setText(str(tau))

        latitude = self.left.ui.latitudeLineEdit.value()    # Phi
        denominator = math.cos(math.radians(tau)) * math.sin(math.radians(latitude)) - math.tan(math.radians(delta)) * math.cos(math.radians(latitude))
        azimut = math.atan( math.sin(math.radians(tau)) / denominator )
        if denominator < 0:
            azimut = azimut + math.radians(180)
        azimut = math.degrees(azimut)
        self.left.ui.azimutALineEdit.setText(str(azimut))

        sunHeight = math.asin( math.cos(math.radians(delta)) * math.cos(math.radians(tau)) * math.cos(math.radians(latitude)) + math.sin(math.radians(delta)) * math.sin(math.radians(latitude)) )
        self.left.ui.sunHeightHLineEdit.setText(str(math.degrees(sunHeight)))

        self.right.setAngle(azimut)

    def saveState(self):
#         # Note: there is no way to have eclipse shutdown the application faithfully,
#         # see also http://stackoverflow.com/questions/677531/is-it-possible-for-eclipse-to-terminate-gently-instead-of-using-sigkill
#         path = self.mainWidget.browserWidget.getCurrentPath()
#         self.settings.setBrowserPath(path)
        self.settings.setMainWindowPos(self.pos())
        self.settings.setMainWindowSize(self.size())
        self.settings.save()
 
 
    def handleAbout(self):
        appVersion = "Development version"
        pythonVersion = "%s.%s.%s (%s)" % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3])
        pyQtVersion = PYQT_VERSION_STR
        pyQtQtVersion = QT_VERSION_STR
        qtRuntimeVersion = qVersion()
         
        platformSystem = platform.system()
        platformRelease = platform.release()
 
        QMessageBox.about(self, "About Shadow",
                          "Copyright \u00a9 2015 by Andreas Fester<br/>"+
                          "<table>"+
                          "<tr><th align=\"right\">Application version:</th><td>{}</td></tr>".format(appVersion) +
                          "<tr><th align=\"right\">Python version:</th><td>{}</td></tr>".format(pythonVersion) +
                          "<tr><th align=\"right\">PyQt version:</th><td>{} for Qt {}</td></tr>".format(pyQtVersion, pyQtQtVersion) +
                          "<tr><th align=\"right\">Qt runtime version:</th><td>{}</td></tr>".format(qtRuntimeVersion)+
                          "<tr><th align=\"right\">Operating System:</th><td>{} {}</td></tr>".format(platformSystem, platformRelease)+
                          "<tr><th align=\"right\">sqlite version:</th><td>{}</td></tr>".format(sqlite3.version) +
                          "<tr><th align=\"right\">sqlite runtime version:</th><td>{}</td></tr>".format(sqlite3.sqlite_version)+
                          "</table>")
