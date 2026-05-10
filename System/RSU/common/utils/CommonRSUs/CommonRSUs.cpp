#include <CommonRSUs.h>
#include <string>
#include <fstream>
#include <rsus/rsuPaths.h>
#include <cstring>
#include <zlibH.h>
#include <filesystem>

const char* GetModel2OperatorMessagesFilePath()
{
	static char szPath[_MAX_PATH * 4] = { 0 };
	if (*szPath == CHAR('\0'))
		snprintf(szPath, sizeof(szPath), "%s_%s", rsuGetMemoryPath().c_str(), "Model2OperatorMessages.datx");
	return szPath;
}

int RsuPackager(LPCSTR pszSrcFile, LPCSTR pszArchive, LPCSTR pszNamePackAs)
{
	zipFile zf = zipOpen(pszArchive, 0);
	if (!zf)
	{
		return -1;
	}

	zip_fileinfo zi;
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
		zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;
	zi.dosDate = filetime(pszSrcFile);

	int err = zipOpenNewFileInZip(zf, pszNamePackAs, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, 8);
	std::ifstream ifs(pszSrcFile, std::ios::binary);
	int allread = 0;
	int coounter = 0;
	if (ifs.is_open())
	{
		char buf[8192];
		memset(buf, 0, sizeof(buf));
		int size_read = 0;
		err = ZIP_OK;
		do
		{
			coounter++;
			ifs.read(buf, sizeof(buf));
			size_read = ifs.gcount();
			err = zipWriteInFileInZip(zf, buf, size_read);
			allread += size_read;
			memset(buf, 0, sizeof(buf));
			if (err != ZIP_OK)
			{
				break;
			}
		} while (!ifs.eof());
	}

	zipCloseFileInZip(zf);
	zipClose(zf, NULL);
	ifs.close();
	return 0;
}

bool RsuExtractor(std::fstream& f, LPCSTR pszArchive)
{
	f.close();
	int err = UNZ_OK;
	unzFile uf = unzOpen(pszArchive);
	if (uf)
	{
		std::string fn = std::string(pszArchive).append(".tmp");
		err = unzOpenCurrentFile(uf);
		if (err == UNZ_OK)
		{
			f.open(fn.c_str(), std::fstream::out | std::fstream::binary);
			if (f.is_open())
			{
				char buf[8192];
				memset(buf, 0, sizeof(buf));
				do
				{
					err = unzReadCurrentFile(uf, buf, sizeof(buf));
					if (err > 0)
					{
						if (!f.write(buf, err))
						{
							f.clear();
							f.close();
							unzClose(uf);
							err = UNZ_ERRNO;
							break;
						}

						memset(buf, 0, sizeof(buf));
					}
				} while (err > 0);
			}
		}

		unzClose(uf);
		f.close();
		f.open(fn.c_str(), std::fstream::in | std::fstream::binary);
		return err == UNZ_OK && f.is_open();
	}

	return false;
}

FILE* RsuExtractor(LPCSTR pszArchive)
{
	FILE* fout = NULL;
	int err = UNZ_OK;

	unzFile uf = unzOpen(pszArchive);
	if (uf)
	{
		std::string fn = std::string(pszArchive) + ".tmp";

		err = unzOpenCurrentFile(uf);
		if (err == UNZ_OK)
		{
			fout = fopen(fn.c_str(), "wb");
			if (fout)
			{
				char buf[8192];
				memset(buf, 0, sizeof(buf));
				do
				{
					err = unzReadCurrentFile(uf, &buf[0], sizeof(buf));
					if (err > 0)
					{
						if (fwrite(&buf[0], err, 1, fout) != 1)
						{
							err = UNZ_ERRNO;
							break;
						}
					}
				} while (err > 0);
				fclose(fout);
				fout = fopen(fn.c_str(), "rb");
			}
		}
		unzClose(uf);
	}
	if (fout && err != UNZ_OK)
	{
		fclose(fout);
		fout = NULL;
	}
	return fout;
}
