'''
Created on 01.09.2015

@author: afester
'''
 
from PyQt5.Qt import QSizeF
from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, Qt, QRectF, QDate, QTime, QTimeZone, QDateTime
from PyQt5.QtWidgets import QWidget
from PyQt5.QtWidgets import QHBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QMenuBar, QMessageBox
from PyQt5.QtGui import QPalette, QPainter, QImage, QBrush
from PyQt5.QtSvg import QSvgRenderer

import math
import sys, os, platform, logging

from ui.Data import Ui_Form

from os.path import expanduser

from Settings import Settings

class ShadowWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.texture = QImage("ui/gras.png")
        self.tree = QSvgRenderer("ui/tree.svg")
        self.compass = QSvgRenderer("ui/compass2.svg")

        # set the gras texture as the background image
        self.setAutoFillBackground(True)
        pal = self.palette()
        pal.setBrush(QPalette.Window, QBrush(self.texture))
        self.setPalette(pal)
        self.angle = 0
        self.shadowLength = 10

    def setAngle(self, value):
        self.angle = value
        self.repaint()

    def setShadowLength(self, value):
        if (value < 0):
            value = 0
        self.shadowLength = value
        self.repaint()

    def paintEvent(self, event):
        radius = 50
        
        treeSize = QSizeF(2 * radius, 2 * radius)
        bounds = QRectF( (self.width() - treeSize.height()) / 2,
                         (self.height() - treeSize.width()) / 2,
                         treeSize.width(),
                         treeSize.height())

        painter = QPainter(self)

        # draw the shadow
        painter.setBrush(Qt.black)
        painter.setPen(Qt.NoPen)
        painter.setOpacity(0.5)

        xrad = 95
        yrad = self.shadowLength * 20
        rect = QRectF(-xrad, -yrad, xrad, yrad)
        painter.translate(self.width() / 2, self.height() / 2)
        painter.rotate(self.angle)
        painter.translate(xrad/2, yrad/2)
        painter.drawChord(rect, 0, 180*16)
        painter.resetTransform()

        # draw the tree
        painter.setOpacity(1)
        self.tree.render(painter, bounds)

        # draw the compass
        bounds = QRectF( 10, 10, 50, 50)
        self.compass.render(painter, bounds)
        #painter.setBrush(Qt.NoBrush)
        #painter.drawRect(bounds)


class DataWidget(QWidget):
    
    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.ui = Ui_Form()
        self.ui.setupUi(self)


class SunSimulator:
    
    def __init__(self):
        pass


    def toString(self):
        return "Date/Time (UTC)        : {}\n" \
               "Julian Day             : {}\n" \
               "Time variable          : {}\n" \
               "Mean ecliptic Length   : {}\n" \
               "Mean anomaly           : {}\n" \
               "Ecliptic Length        : {}\n" \
               "Ecliptic skew          : {}\n" \
               "Right ascention        : {}\n" \
               "Declination            : {}\n" \
               "Julian day (Date)      : {}\n" \
               "Time variable (Date)   : {}\n" \
               "Mean GMT star date     : {}\n" \
               "GMT spring angle       : {}\n" \
               "Spring hour angle      : {}\n" \
               "Sun's hour angle       : {}\n" \
               "Azimut                 : {}\n" \
               "Sun's height angle     : {}\n" \
               "Shadow angle           : {}\n" \
               "Shadow length          : {}\n" \
               "Time difference        : {}\n" \
               "Right ascention (hours): {}\n" \
               "Mean Sun's right ascen.: {}\n" \
               "Time equation          : {}\n" \
               "Sun rise (WOZ)         : {}\n" \
               "Sun set (WOZ)          : {}\n" \
               "Sun rise (MOZ)         : {}\n" \
               "Sun set (MOZ)          : {}\n" \
               "".format(self.dateTimeUtc, self.JD, self.n, self.L, self.g,
                         self.Lambda, self.epsilon, self.alpha, self.delta,
                         self.JD_0, self.T_0, self.ThetaH_G, self.Theta_G,
                         self.Theta, self.tau, self.azimut, self.sunHeight,
                         self.shadowAngle, self.shadowLength, self.Zeitdifferenz,
                         self.RA, self.RAm, self.Zeitgleichung, self.AufgangOrtszeit,
                         self.UntergangOrtszeit, self.Aufgang, self.Untergang)


    def recalculate(self, dateTime, longitude, latitude, treeHeight):
        # Convert given date/time to UTC
        self.dateTimeUtc = dateTime.toTimeSpec(Qt.UTC)

        # Calculate Julian day number
        self.JD = float(self.dateTimeUtc.date().toJulianDay())
        minuteOfDay = self.dateTimeUtc.time().msecsSinceStartOfDay() / (60 * 1000)
        timeFraction = minuteOfDay / (24 * 60) - 0.5    # time base on 01/01/2000 is 12:00 !!!!
        self.JD = self.JD + timeFraction

        # Time variable n
        self.n = self.JD - 2451545.0

        # Mean ecliptic Length
        self.L = 280.460 + 0.9856474 * self.n
        divis = int(self.L / 360)
        self.L = self.L - divis * 360

        # Mean anomaly
        self.g = 357.528 +0.9856003 * self.n
        divis = int(self.g / 360)
        self.g = self.g - divis * 360

        # Ecliptic Length
        self.Lambda = self.L + 1.915 * math.sin(math.radians(self.g)) + 0.01997 * math.sin(math.radians(2 * self.g)) 

        # Ecliptic skew
        self.epsilon = 23.439 - 0.0000004 * self.n

        # Rektaszension 
        denominator = math.cos(math.radians(self.Lambda))
        self.alpha = math.atan( math.cos(math.radians(self.epsilon)) * math.sin(math.radians(self.Lambda)) / denominator )
        if denominator < 0:
            self.alpha = self.alpha + math.radians(180)
        self.alpha = math.degrees(self.alpha)

        # Deklination
        self.delta = math.asin(math.sin(math.radians(self.epsilon)) * math.sin(math.radians(self.Lambda)))
        self.delta = math.degrees(self.delta)

        # Julian day (Date)
        self.JD_0 = float(self.dateTimeUtc.date().toJulianDay()) - 0.5   # time base on 01/01/2000 is 12:00 !!!!
        
        # Time variable (Date)
        self.T_0 = (self.JD_0 - 2451545.0) / 36525

        # Time of day (in fractional hours)
        self.T = self.dateTimeUtc.time().msecsSinceStartOfDay() / (60 * 60 * 1000)

        # Mean Greenwich star date
        self.ThetaH_G = 6.697376 + 2400.05134 * self.T_0 + 1.002738 * self.T
        divis = int(self.ThetaH_G / 24)
        self.ThetaH_G = self.ThetaH_G - divis * 24

        # Mean Greenwich spring angle
        self.Theta_G = self.ThetaH_G * 15

        # Spring hour angle
        self.Theta = self.Theta_G + longitude
        divis = int(self.Theta / 360)
        self.Theta = self.Theta - divis * 360

        # Sun's Hour angle
        self.tau = self.Theta - self.alpha

        # Azimut
        denominator = math.cos(math.radians(self.tau)) * math.sin(math.radians(latitude)) - math.tan(math.radians(self.delta)) * math.cos(math.radians(latitude))
        self.azimut = math.atan( math.sin(math.radians(self.tau)) / denominator )
        if denominator < 0:
            self.azimut = self.azimut + math.radians(180)
        self.azimut = math.degrees(self.azimut)

        # Sun's height angle
        self.sunHeight = math.asin( math.cos(math.radians(self.delta)) * math.cos(math.radians(self.tau)) * math.cos(math.radians(latitude)) + math.sin(math.radians(self.delta)) * math.sin(math.radians(latitude)) )

        # shadow angle
        self.shadowAngle = -self.azimut

        # shadow length
        self.shadowLength = treeHeight / math.tan(self.sunHeight) 

        # Time difference
        self.Zeitdifferenz = 12*math.acos((math.sin(-0.0145) - math.sin(math.radians(latitude))*math.sin(math.radians(self.delta))) / (math.cos(math.radians(latitude))*math.cos(math.radians(self.delta))))/math.pi

        # Right ascention converted to hours 
        self.RA = 24.0*math.radians(self.alpha)/(2*math.pi)

        # Mean Sun's Right ascention
        self.RAm = 18.71506921 + 2400.0513369*self.T_0 +(2.5862e-5 - 1.72e-9*self.T_0)*self.T_0*self.T_0
        divis = int(self.RAm / 24)
        self.RAm = self.RAm - 24*divis

        # Time equation
        self.Zeitgleichung = 1.0027379*(self.RAm - self.RA)

        # Sun rise and sun set (WOZ)
        self.AufgangOrtszeit = 12 - self.Zeitdifferenz - self.Zeitgleichung
        self.UntergangOrtszeit = 12 + self.Zeitdifferenz - self.Zeitgleichung

        # Sun rise and sun set (MOZ)
        self.Aufgang = self.AufgangOrtszeit - longitude/15 + 2
        self.Untergang = self.UntergangOrtszeit - longitude/15 + 2



class MainWindow(QMainWindow):
# 
    l = logging.getLogger('MainWindow')
    # timeZones = [ ('UTC-12', -12), ('UTC-11', -11), ('UTC', 0), ('UTC+1', 1), ('UTC+2', 2)]

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.l.debug('Initializing MainWindow ...')

        self.theSun = SunSimulator()

        self.timeZones = []
        for x in QTimeZone.availableTimeZoneIds():
            tz = QTimeZone(x)
            self.timeZones.append(tz)
            #print(" {} {} {}".format(str(tz.id()), tz.comment(), tz.hasDaylightTime()))

        #self.revLookup = {}
        #idx = 0
        #for d in self.timeZones:
        #    self.revLookup[d[1]] = idx
        #    idx += 1

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

        self.centralWidget = QWidget()
        self.left = DataWidget(self.centralWidget)
        self.right = ShadowWidget(self.centralWidget)

        # Create time zone selector
        self.left.ui.timeZoneComboBox.clear()
        for tz in self.timeZones:
            # TODO: Hack (?)
            displayId = tz.id().data().decode(encoding="UTF-8")
            if not displayId.startswith('UTC'):
                utcOffset = tz.displayName(QTimeZone.GenericTime, QTimeZone.OffsetName)
                #self.left.ui.timeZoneComboBox.addItem("({}) {} - {}".format(utcOffset, displayId, tz.comment()))
                self.left.ui.timeZoneComboBox.addItem("({}) {}".format(utcOffset, displayId))

        self.left.ui.timeZoneComboBox.setCurrentIndex(self.settings.getTzIndex())
        self.left.ui.timeZoneComboBox.currentIndexChanged.connect(self.setTimeZone)

        self.left.ui.dateEdit.dateChanged.connect(self.updateDate)
        self.left.ui.timeEdit.timeChanged.connect(self.updateTime)
        self.left.ui.timeSlider.valueChanged.connect(self.slideTime)

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

        # sample values
        self.left.ui.treeHeightLineEdit.setValue(10)
        self.left.ui.latitudeLineEdit.setValue(48.1)
        self.left.ui.LongitudeLineEdit.setValue(11.6)
        self.left.ui.dateEdit.setDate(QDate(2015, 9, 11))
        self.left.ui.timeEdit.setTime(QTime(8, 0))
        #self.left.ui.latitudeLineEdit.setValue(50)
        #self.left.ui.LongitudeLineEdit.setValue(10)
        #self.left.ui.dateEdit.setDate(QDate(2005, 9, 30))
        #self.left.ui.timeEdit.setTime(QTime(12, 0))


    def updateDate(self, date):
        #self.date = date
        self.recalculate()

    def updateTime(self, time):
        #self.time = time
        self.recalculate()

    def setTimeZone(self, idx):
        tz = self.timeZones[idx]
        print("Time zone: {}", tz)
        self.recalculate()

    def slideTime(self, value):
        minute = value % 60
        hour = value // 60
        self.left.ui.timeEdit.setTime(QTime(hour, minute))

    def recalculate(self):
        longitude = self.left.ui.LongitudeLineEdit.value()  # lambda
        latitude = self.left.ui.latitudeLineEdit.value()    # Phi
        treeHeight = int(self.left.ui.treeHeightLineEdit.value())

        # Step 1: calculate sun set and sun rise
        dateTime = QDateTime(self.left.ui.dateEdit.date(), QTime(12,0), Qt.UTC)
        self.theSun.recalculate(dateTime, longitude, latitude, treeHeight)

        sunRise = QTime(int(self.theSun.Aufgang),  int(60.0*(self.theSun.Aufgang - int(self.theSun.Aufgang))))
        sunSet = QTime(int(self.theSun.Untergang), int(60.0*(self.theSun.Untergang - int(self.theSun.Untergang))))
        self.left.ui.sunriseLineEdit.setText(sunRise.toString(Qt.ISODate))
        self.left.ui.sunsetLineEdit.setText(sunSet.toString(Qt.ISODate))
        minuteStart = sunRise.msecsSinceStartOfDay() / 1000 / 60 
        minuteEnd = sunSet.msecsSinceStartOfDay() / 1000 / 60 
        self.left.ui.timeSlider.setMinimum(minuteStart)
        self.left.ui.timeSlider.setMaximum(minuteEnd)
        
        # Construct a time zone. NOTE: use IDs instead
        tzIdx = self.left.ui.timeZoneComboBox.currentIndex()
        timeZone = self.timeZones[tzIdx]
        dateTime = QDateTime(self.left.ui.dateEdit.date(), self.left.ui.timeEdit.time(), timeZone)

        self.theSun.recalculate(dateTime, longitude, latitude, treeHeight)
        self.left.ui.timeUTCLineEdit.setText(self.theSun.dateTimeUtc.toString(Qt.ISODate))
        self.left.ui.jDLineEdit.setText(str(self.theSun.JD))
        self.left.ui.timeVariableNLineEdit.setText(str(self.theSun.n))
        self.left.ui.LVariableLineEdit.setText(str(self.theSun.L))
        self.left.ui.meanAnomalyGLineEdit.setText(str(self.theSun.g))
        self.left.ui.eclipticLengthLineEdit.setText(str(self.theSun.Lambda))
        self.left.ui.eclipticSkewLineEdit.setText(u"{}\u00B0 / {} rad".format(self.theSun.epsilon, math.radians(self.theSun.epsilon)))
        self.left.ui.rightAscensionLineEdit.setText(u"{}\u00B0 / {} rad".format(self.theSun.alpha, math.radians(self.theSun.alpha)))
        self.left.ui.declinationLineEdit.setText(u"{}\u00B0 / {} rad".format(self.theSun.delta, math.radians(self.theSun.delta)))
        self.left.ui.jD_0LineEdit.setText(str(self.theSun.JD_0))
        self.left.ui.t_0LineEdit.setText(str(self.theSun.T_0))
        self.left.ui.sternzeitH_GLineEdit.setText(str(self.theSun.ThetaH_G))
        self.left.ui.greenwichStundenwinkelFP_GLineEdit.setText(str(self.theSun.Theta_G))
        self.left.ui.stundenwinkelFrHlingspunktLineEdit.setText(str(self.theSun.Theta))
        self.left.ui.stundenwinkelLineEdit.setText(str(self.theSun.tau))

        self.left.ui.azimutALineEdit.setText(u"{:.1f}\u00B0".format(self.theSun.azimut))
        self.left.ui.sunHeightHLineEdit.setText(u"{:.1f}\u00B0".format(math.degrees(self.theSun.sunHeight)))
        self.left.ui.shadowLengthLineEdit.setText("{:.1f} m".format(self.theSun.shadowLength))

        self.right.setAngle(self.theSun.azimut) # shadowAngle)
        self.right.setShadowLength(self.theSun.shadowLength)


    def saveState(self):
#         # Note: there is no way to have eclipse shutdown the application faithfully,
#         # see also http://stackoverflow.com/questions/677531/is-it-possible-for-eclipse-to-terminate-gently-instead-of-using-sigkill
        self.settings.setTzIdx(self.left.ui.timeZoneComboBox.currentIndex())
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
                          "</table>")
