#include <windows.h>
#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>
#include <cctype>
#include <vector>
#include <fstream>

typedef std::pair<float, float> coordType;
typedef std::vector<coordType> coordVector;

float convert(Exiv2::ExifData::const_iterator& iter)
{
	float val = 0.0f;
	const long n = iter->value().count();
	for (long i = 0; i < n; i++)
	{
		const int32_t d = iter->value().toRational(i).second;
		if (d == 0)
		{
			return 0.0f;
		}
		const int32_t z = iter->value().toRational(i).first;
		val += static_cast<float>(z)/d/pow(static_cast<float>(60), i);
	}
	return val;
}

wchar_t* getEnvVar(const wchar_t* wEnvVar)
{
	size_t requiredSize;
	_wgetenv_s(&requiredSize, nullptr, 0, wEnvVar);
	wchar_t* wEnvVar2 = new wchar_t[requiredSize];
	if (!wEnvVar2)
	{
		wchar_t msg[] = L"Failed to allocate memory!";
		std::wcout << msg << L"\n";
		//MessageBoxW(nullptr, msg, L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}
	// get the value of the environment variable
	_wgetenv_s(&requiredSize, wEnvVar2, requiredSize, wEnvVar);
	return wEnvVar2;
}

void replaceAll(std::wstring& str, const std::wstring& from, const std::wstring& to)
{
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc <= 1)
	{
		wchar_t wff[_MAX_FNAME];
		wchar_t wfe[_MAX_EXT];
		_wsplitpath_s(argv[0], nullptr, 0, nullptr, 0, wff, _MAX_FNAME, wfe, _MAX_EXT);
		const std::wstring msg = std::wstring(L"Usage:\n") + std::wstring(wff) + std::wstring(L"[") +  std::wstring(wfe) + std::wstring(L"] ")
			+ std::wstring(L"[<params>] [pic1 .. picN]\n")
			+ std::wstring(L"\n")
			+ std::wstring(L"Display GPSInfo from a picture (collection) in Google Earth or web browser\n")
			+ std::wstring(L"\n")
			+ std::wstring(L"Parameters:\n")
			+ std::wstring(L"-b browser.exe - to set path to web browser application\n")
			+ std::wstring(L"-ge googleearth.exe - to set path to Google Earth application, defaults to\n")
			+ std::wstring(L"  \"%ProgramFiles%\\Google\\Google Earth\\client\\googleearth.exe\" or\n")
			+ std::wstring(L"  \"%ProgramFiles%\\Google\\Google Earth\\googleearth.exe\"\n")
			+ std::wstring(L"-l input.lst - to set path of input picture list file\n")
			+ std::wstring(L"-nge - to not start Google Earth application\n")
			+ std::wstring(L"-o output.kml - to set path of KML output file, defaults to %TEMP%\\name.kml\n")
			+ std::wstring(L"-url \"http://geomap.url\" - to set the template geo/map URL to open in browser,\n")
			+ std::wstring(L"  defaults to \"http://maps.google.com/?q=#LAT#,#LON#\" where\n")
			+ std::wstring(L"  #LAT#/#LON# is replaced by the signed latitude/longitude, resp.\n")
			+ std::wstring(L"  #LATABS#/#LONABS# # is replaced by the unsigned latitude/longitude, resp.\n")
			+ std::wstring(L"  #LATREF# is replaced by the latitude reference, e.g. N or S\n")
			+ std::wstring(L"  #LONREF# is replaced by the longitude reference, e.g. E or W\n")
			+ std::wstring(L"-v - to show program version and exit\n")
			+ std::wstring(L"\n")
			+ std::wstring(L"Examples:\n")
			+ std::wstring(wff) + std::wstring(L" -o \"C:\\temp\\") + std::wstring(wff) + std::wstring(L".kml\" DSC1.jpg DSC2.jpg DSC3.jpg\n")
			+ std::wstring(wff) + std::wstring(L" -l \"C:\\temp\\pics.lst\"\n")
			+ std::wstring(wff) + std::wstring(L" -nge -b firefox -url \"http://osm.org/?mlat=#LAT#&mlon=#LON#\" DSC1.jpg DSC2.jpg\n")
			+ std::wstring(wff) + std::wstring(L" -b chrome -url \"http://toolserver.org/~geohack/geohack.php?params=#LATABS#_#LATREF#_#LONABS#_#LONREF#\" DSC1.jpg");
		std::wcout << msg << L"\n";
		MessageBoxW(nullptr, msg.c_str(), wff, MB_ICONINFORMATION | MB_OK);
		return 0;
	}

	std::wstring wBrowserPath;
	std::wstring wGEPath;
	std::wstring wKMLPath;
	std::wstring wURL(L"http://maps.google.com/?q=#LAT#,#LON#");
	std::vector<std::wstring> images;
	bool bGE = true;
	int i;
	int iStart = 1;
	for (i = iStart; i < argc; i++)
	{
		std::wstring flag(argv[i]);
		if (argc == 2 && (flag.compare(L"-v") == 0 || flag.compare(L"-V") == 0))
		{
			wchar_t wff[_MAX_FNAME];
			wchar_t wfe[_MAX_EXT];
			_wsplitpath_s(argv[0], nullptr, 0, nullptr, 0, wff, _MAX_FNAME, wfe, _MAX_EXT);
			const std::wstring msg = std::wstring(wff) + std::wstring(L" 1.0.5.0\n")
				+ std::wstring(L"Copyright (C) 2010-2013  Thomas Beutlich\n")
				+ std::wstring(L"\n")
				+ std::wstring(L"This program is free software: you can redistribute it and/or modify\n")
				+ std::wstring(L"it under the terms of the GNU General Public License as published by\n")
				+ std::wstring(L"athe Free Software Foundation, either version 3 of the License, or\n")
				+ std::wstring(L"(at your option) any later version.\n")
				+ std::wstring(L"\n")
				+ std::wstring(L"This program is distributed in the hope that it will be useful,\n")
				+ std::wstring(L"but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
				+ std::wstring(L"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
				+ std::wstring(L"GNU General Public License for more details.\n")
				+ std::wstring(L"\n")
				+ std::wstring(L"You should have received a copy of the GNU General Public License\n")
				+ std::wstring(L"along with this program.  If not, see <http://www.gnu.org/licenses/>.");
			std::wcout << msg << L"\n";
			return 0;
		}
		else if (flag.compare(L"-ge") == 0 || flag.compare(L"-gE") == 0 || flag.compare(L"-Ge") == 0 || flag.compare(L"-GE") == 0)
		{
			if (argc > i)
			{
				wGEPath = std::wstring(argv[++i]);
				iStart += 2;
			}
		}
		else if (flag.compare(L"-b") == 0 || flag.compare(L"-B") == 0)
		{
			if (argc > i)
			{
				wBrowserPath = std::wstring(argv[++i]);
				iStart += 2;
			}
		}
		else if (flag.compare(L"-o") == 0 || flag.compare(L"-O") == 0)
		{
			if (argc > i)
			{
				wchar_t* filePart;
				DWORD pathSize = GetFullPathNameW(argv[++i], 0, nullptr, &filePart);
				wchar_t* lpBuf = new wchar_t[pathSize];
				GetFullPathNameW(argv[i], pathSize, lpBuf, &filePart);
				wKMLPath = std::wstring(lpBuf);
				delete [] lpBuf;
				iStart += 2;
			}
		}
		else if (flag.compare(L"-url") == 0 || flag.compare(L"-URL") == 0)
		{
			if (argc > i)
			{
				wURL = std::wstring(argv[++i]);
				iStart += 2;
			}
		}
		else if (flag.compare(L"-nge") == 0 || flag.compare(L"-ngE") == 0 || flag.compare(L"-nGe") == 0 || flag.compare(L"-nGE") == 0)
		{
			bGE = false;
			iStart++;
		}
		else if (flag.compare(L"-l") == 0 || flag.compare(L"-L") == 0)
		{
			if (argc > i)
			{
				wchar_t* filePart;
				DWORD pathSize = GetFullPathNameW(argv[++i], 0, nullptr, &filePart);
				wchar_t* lpBuf = new wchar_t[pathSize];
				GetFullPathNameW(argv[i], pathSize, lpBuf, &filePart);
				std::wifstream listFile(lpBuf);
				delete [] lpBuf;
				if (listFile.fail())
				{
					wchar_t msg[] = L"Cannot open list file!";
					std::wcout << msg << L"\n";
					//MessageBoxW(nullptr, msg, L"Error", MB_ICONERROR | MB_OK);
				}
				else
				{
					std::wstring line;
					while (std::getline(listFile, line))
					{
						images.push_back(line);
					}
					listFile.close();
				}
				iStart += 2;
			}
		}
	}

	if (wKMLPath.empty())
	{
		// get temporary file name
		wchar_t tempFileName[L_tmpnam_s];
		errno_t err = _wtmpnam_s(tempFileName, L_tmpnam_s);
		if (err)
		{
			wchar_t msg[] = L"Error occurred creating unique temporary file name!";
			std::wcout << msg << L"\n";
			//MessageBoxW(nullptr, msg, L"Error", MB_ICONERROR | MB_OK);
			return 0;
		}
		wchar_t* wTEMP = getEnvVar(L"TEMP");
		wKMLPath = std::wstring(wTEMP) + std::wstring(tempFileName) + std::wstring(L"kml");
		delete [] wTEMP;
	}

	for (i = iStart; i < argc; i++)
	{
		wchar_t* filePart;
		DWORD pathSize = GetFullPathNameW(argv[i], 0, nullptr, &filePart);
		wchar_t* lpBuf = new wchar_t[pathSize];
		GetFullPathNameW(argv[i], pathSize, lpBuf, &filePart);
		images.push_back(lpBuf);
		delete [] lpBuf;
	}

	// open (temporary) kml file
	FILE* fp;
	errno_t err = _wfopen_s(&fp, wKMLPath.c_str(), L"w, ccs=UTF-8");
	if (err || !fp)
	{
		wchar_t msg[] = L"Cannot open (temporary) KML file!";
		std::wcout << msg << L"\n";
		//MessageBoxW(nullptr, msg, L"Error", MB_ICONERROR | MB_OK);
	}
	else
	{
		fwprintf(fp, L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		fwprintf(fp, L"<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
		fwprintf(fp, L"<Document>\n");
		fwprintf(fp, L"<open>1</open>\n");
	}

	coordVector coords;
	std::vector<std::wstring>::iterator file;
	for (file = images.begin(); file != images.end(); ++file)
	{
		try
		{
			Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(file->c_str());
			if (image->good())
			{
				image->readMetadata();
				Exiv2::ExifData &exifData = image->exifData();

				// latitude
				Exiv2::ExifKey exifKeyLat("Exif.GPSInfo.GPSLatitude");
				Exiv2::ExifData::const_iterator iter = exifData.findKey(exifKeyLat);
				if (iter == exifData.end())
				{
					continue;
				}
				float lat = convert(iter);
				exifKeyLat = Exiv2::ExifKey("Exif.GPSInfo.GPSLatitudeRef");
				iter = exifData.findKey(exifKeyLat);
				if (iter == exifData.end())
				{
					continue;
				}
				else if (iter->value().toString().compare("S") == 0)
				{
					lat = -lat;
				}

				// longitude
				Exiv2::ExifKey exifKeyLon("Exif.GPSInfo.GPSLongitude");
				iter = exifData.findKey(exifKeyLon);
				if (iter == exifData.end())
				{
					continue;
				}
				float lon = convert(iter);
				exifKeyLon = Exiv2::ExifKey("Exif.GPSInfo.GPSLongitudeRef");
				iter = exifData.findKey(exifKeyLon);
				if (iter == exifData.end())
				{
					continue;
				}
				else if (iter->value().toString().compare("W") == 0)
				{
					lon = -lon;
				}

				if (fp)
				{
					// altitude
					float alt = 0.0f;
					Exiv2::ExifKey exifKeyAlt("Exif.GPSInfo.GPSAltitude");
					iter = exifData.findKey(exifKeyAlt);
					if (iter != exifData.end())
					{
						alt = iter->toFloat();
						exifKeyAlt = Exiv2::ExifKey("Exif.GPSInfo.GPSAltitudeRef");
						iter = exifData.findKey(exifKeyAlt);
						if (iter != exifData.end())
						{
							if (iter->value().toString().compare("1") == 0)
							{
								alt = -alt;
							}
						}
					}

					// dimension
					Exiv2::ExifKey exifKeyX("Exif.Photo.PixelXDimension");
					std::wstring wh(L"width");
					iter = exifData.findKey(exifKeyX);
					if (iter != exifData.end())
					{
						float width = iter->toFloat();
						Exiv2::ExifKey exifKeyY("Exif.Photo.PixelYDimension");
						iter = exifData.findKey(exifKeyY);
						if (iter != exifData.end())
						{
							float height = iter->toFloat();
							wh = width > height ? L"width" : L"height";
						}
					}

					wchar_t wff[_MAX_FNAME];
					wchar_t wfe[_MAX_EXT];
					_wsplitpath_s(file->c_str(), nullptr, 0, nullptr, 0, wff, _MAX_FNAME, wfe, _MAX_EXT);
					std::wstring wfeLower(wfe);
					for (unsigned i = 0; i < wfeLower.length(); i++)
					{
						wfeLower[i] = std::tolower(wfe[i]);
					}
					const std::wstring fileName = std::wstring(wff) + std::wstring(wfe);
					fwprintf(fp, L"<Placemark>\n<name>%s</name>\n", fileName.c_str());
					fwprintf(fp, L"<description><![CDATA[<b>Lat:</b> %.6f, <b>Lon:</b> %.6f, <b>Alt:</b> %.1f<br/>", lat, lon, alt);
					fwprintf(fp, L"<b>File:</b> %s<br/>", file->c_str());
					std::string comment = image->comment();
					if (!comment.empty())
					{
						fwprintf(fp, L"<b>Comment:</b> %s<br/>", Exiv2::s2ws(comment).c_str());
					}
					std::replace(file->begin(), file->end(), '\\', '/');
					fwprintf(fp, L"<br/><img src=\"file:///%s\" %s=\"480\"/><hr/>]]>", (file->substr(0, file->length() - wfeLower.length()) + wfeLower).c_str(), wh.c_str());
					fwprintf(fp, L"</description>\n<Point>\n<coordinates>%.6f,%.6f,%.1f</coordinates>\n</Point>\n</Placemark>\n", lon, lat, alt);
				}
				coords.push_back(std::make_pair(lon, lat));
			}
		}
		catch (const Exiv2::WError& e)
		{
			std::wcout << e.wwhat() << L"\n";
			continue;
		}
		catch (const std::exception& e)
		{
			std::wcout << Exiv2::s2ws(e.what()).c_str() << L"\n";
			continue;
		}
		catch (...)
		{
			continue;
		}
	}

	if (fp)
	{
		fwprintf(fp, L"</Document>\n");
		fwprintf(fp, L"</kml>\n");
		fclose(fp);
	}

	BOOL retVal = TRUE;

	if (!coords.empty())
	{
		if (bGE && !err)
		{
			// find path to Google Earth
			WIN32_FIND_DATAW findData;
			SecureZeroMemory(&findData, sizeof(findData));
			if (wGEPath.empty())
			{
				wchar_t* wProgramFiles = getEnvVar(L"ProgramFiles");
				wGEPath = std::wstring(wProgramFiles) + std::wstring(L"\\Google\\Google Earth\\client\\googleearth.exe");
				if (FindFirstFileW(wGEPath.c_str(), &findData) == INVALID_HANDLE_VALUE)
				{
					wGEPath = std::wstring(wProgramFiles) + std::wstring(L"\\Google\\Google Earth\\googleearth.exe");
				}
				delete [] wProgramFiles;
			}

			// final check path to Google Earth
			SecureZeroMemory(&findData, sizeof(findData));
			if (FindFirstFileW(wGEPath.c_str(), &findData) == INVALID_HANDLE_VALUE)
			{
				wchar_t msg[] = L"Cannot find Google Earth!";
				std::wcout << msg << L"\n";
				//MessageBoxW(nullptr, msg, L"Warning", MB_ICONWARNING | MB_OK);
			}
			else
			{
				// build command line and start Google Earth
				const std::wstring wCommandLine = std::wstring(L"\"") + wGEPath + std::wstring(L"\" \"") + wKMLPath + std::wstring(L"\"");
				STARTUPINFOW si;
				SecureZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				PROCESS_INFORMATION pi;
				SecureZeroMemory(&pi, sizeof(pi));
				retVal = CreateProcessW(wGEPath.c_str(), const_cast<wchar_t*>(wCommandLine.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
			}
		}

		if (!wBrowserPath.empty())
		{
			// check path to browser
			WIN32_FIND_DATAW findData;
			SecureZeroMemory(&findData, sizeof(findData));
			if (FindFirstFileW(wBrowserPath.c_str(), &findData) == INVALID_HANDLE_VALUE)
			{
				wchar_t msg[] = L"Cannot find web browser!";
				std::wcout << msg << L"\n";
				//MessageBoxW(nullptr, msg, L"Warning", MB_ICONWARNING | MB_OK);
			}
			else
			{
				coordVector::const_iterator iter;
				for (iter = coords.begin(); iter != coords.end(); ++iter)
				{
					//std::wstring wCommandArg(L"http://maps.google.com/?q=#LAT#,#LON#");
					//std::wstring wCommandArg(L"http://toolserver.org/~geohack/geohack.php?language=de&params=#LATABS#_#LATREF#_#LONABS#_#LONREF#");
					std::wstring wCommandArg = wURL;
					wchar_t wTmp[101];
					replaceAll(wCommandArg, L"#LONREF#", iter->first > 0 ? L"E" : L"W");
					_snwprintf(wTmp, 100, L"%.6f", fabs(iter->first));
					replaceAll(wCommandArg, L"#LONABS#", wTmp);
					_snwprintf(wTmp, 100, L"%.6f", iter->first);
					replaceAll(wCommandArg, L"#LON#", wTmp);
					replaceAll(wCommandArg, L"#LATREF#", iter->second > 0 ? L"N" : L"S");
					_snwprintf(wTmp, 100, L"%.6f", fabs(iter->second));
					replaceAll(wCommandArg, L"#LATABS#", wTmp);
					_snwprintf(wTmp, 100, L"%.6f", iter->second);
					replaceAll(wCommandArg, L"#LAT#", wTmp);

					// build command lines and start browser
					const std::wstring wCommandLine = std::wstring(L"\"") + wBrowserPath + std::wstring(L"\" \"") + wCommandArg + std::wstring(L"\"");
					STARTUPINFOW si;
					SecureZeroMemory(&si, sizeof(si));
					si.cb = sizeof(si);
					PROCESS_INFORMATION pi;
					SecureZeroMemory(&pi, sizeof(pi));
					retVal = CreateProcessW(wBrowserPath.c_str(), const_cast<wchar_t*>(wCommandLine.c_str()), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);
				}
			}
		}
	}

	coords.clear();
	images.clear();

	return retVal;
}
