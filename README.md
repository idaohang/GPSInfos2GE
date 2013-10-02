# GPSInfos 2 Google Earth 1.0.5.0

Copyright © 2010-2013  Thomas Beutlich

## Disclaimer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Description

GPSInfos2GE is a small command line program that extracts the GPSInfo values of the
Exif section of a pictures from digital cameras and displays them in Google Earth
or a web browser.

GPSInfos2GE uses the Exiv2 Image Metadata Library Vs. 0.23.

## Installation Remarks

* GPSInfos2GE.exe can be used with a button in the toolbar of Total Commander. 
  You can copy the following code, adapt the pathes to your installation directory
  and paste it in your default toolbar.

        < ----------------- Copy (Do not include this line) ------------------ >
        TOTALCMD#BAR#DATA
        %COMMANDER_PATH%\Tools\GPSInfos2GE\GPSInfos2GE.exe
        -l %UL
        %COMMANDER_PATH%\TOTALCMD.EXE,3
        GPSInfos2GE
        %COMMANDER_PATH%\Tools\GPSInfos2GE\
        
        -1
        
        < ------------- End copy (Do not include this line) ------------------ >

* GPSInfos2GE.exe can be used as Send To utility. Place a shortcut (CTRL+SHIFT+F5)
  of GPSInfos2GE.exe in directory
 * `%USERPROFILE%\SendTo` if you use Win2k or WinXP,
 * `%APPDATA%\Microsoft\Windows\SendTo` if you use Vista, Win7 or Win8.

## Usage

GPSInfos2GE[.exe] [<params>] [pic1 .. picN]

### Parameters

| Parameter          | Setting  |
| :----------------- | :------- |
| -b browser.exe | to set path to web browser application |
| -ge googleearth.exe | to set path to Google Earth application,<br/>defaults to `"%ProgramFiles%\Google\Google Earth\client\googleearth.exe"` or `"%ProgramFiles%\Google\Google Earth\googleearth.exe"` |
| -l input.lst | to set path of input picture list file |
| -nge | to not start Google Earth application |
| -o output.kml | to set path of KML output file,<br/>defaults to `%TEMP%\name.kml` |
| -url `"http://geomap.url"` | to set the template geo/map URL to open in browser,<br/>defaults to `"http://maps.google.com/?q=#LAT#,#LON#"` where<br/>`#LAT#`/`#LON#` is replaced by the signed latitude/longitude, resp.<br/>`#LATABS#`/`#LONABS#` is replaced by the unsigned latitude/longitude, resp.<br/>`#LATREF#` is replaced by the latitude reference, e.g. N or S<br/>`#LONREF#` is replaced by the longitude reference, e.g. E or W |
| -v | to show program version and exit |

### Examples

* GPSInfos2GE -o `"C:\temp\GPSInfos2GE.kml"` DSC1.jpg DSC2.jpg DSC3.jpg
* GPSInfos2GE -l `"C:\temp\pics.lst"`
* GPSInfos2GE -nge -b firefox -url `"http://osm.org/?mlat=#LAT#&mlon=#LON#"` DSC1.jpg DSC2.jpg
* GPSInfos2GE -b chrome -url `"http://toolserver.org/~geohack/geohack.php?params=#LATABS#_#LATREF#_#LONABS#_#LONREF#"` DSC1.jpg

## ChangeLog

* Version 1.0.5.0 (28.01.2013)
 * added version and license information (option -v)
* Version 1.0.4.0 (23.01.2013)
 * fixed support of option -nge
* Version 1.0.3.0 (22.01.2013)
 * added support of list input (option -l)
 * added support of output file path and name (option -o)
 * added support of option -nge
* Version 1.0.2.0 (17.01.2013)
 * added support of web browser (opions -b and -url)
* Version 1.0.1.0 (12.04.2012)
 * maintenance release
* Version 1.0.0.4 (10.10.2010)
 * first public version

## Acknowledgments

* Exiv2 Image Metadata Library
 * <http://www.exiv2.org>

## Trademark and Copyright Statements

* Total Commander is Copyright © 1993-2013 by Christian Ghisler, Ghisler Software GmbH.
 * <http://www.ghisler.com>

## Feedback

If you have problems, questions, suggestions please contact Thomas Beutlich.
* Email: <support@tbeu.de>
* URL: <http://tbeu.totalcmd.net>
