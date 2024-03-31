#include <Windows.h>

#include <stdint.h>
#include <stdio.h>

unsigned int generate_string(const char *table, unsigned int tablesize, char *str, unsigned int m, unsigned int n)
{
	unsigned int i;

	if (n == 0)
	{
		n++;
	}
	n = rand() % n;
	n++;
	n += m;
	for (i = 0; i < n; i++)
	{
		str[i] = table[rand() % tablesize];
	}
	str[i] = '\0';
	return(i);
}

int wmain(int argc, WCHAR **argv)
{
	const char *table = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		"!@#$%^&*()"
		"-=_+"
		"`~[]\\{}|"
		";':\""
		",./<>?"
		" ";
	unsigned int tablesize;
	unsigned char buffer[65536];
	char str[256];
	unsigned int l;
	unsigned int l0;
	unsigned int l1;
	unsigned int o;
	unsigned int buffersize;
	uint64_t offset;
	uint64_t size;

	tablesize = strlen(table);

	if (argc > 2)
	{
		size = _wtoi64(argv[2]);

		if (size)
		{
			HANDLE hfile;
			DWORD numberofbytes;

			buffersize = sizeof(buffer);
			o = 0;

			unsigned int tickcount0;
			unsigned int tickcount1;

			tickcount0 = GetTickCount();

			hfile = CreateFile(argv[1], GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hfile != INVALID_HANDLE_VALUE)
			{
				offset = 0;
				while (offset < size)
				{
					l = generate_string(table, tablesize, str, 6, 18);

					offset += l;

					l0 = buffersize - o;
					l1 = l < l0 ? l : l0;
					if (l1)
					{
						memcpy(buffer + o, str, l1);
						o += l1;
						if (o < buffersize)
						{
							buffer[o++] = '\n';
							l1++;
						}
					}
					if (o == buffersize)
					{
						WriteFile((HANDLE)hfile, buffer, o, &numberofbytes, NULL);
						o = 0;
					}
					if (l1 < l + 1)
					{
						if (l1 < l)
						{
							memcpy(buffer + o, str + l1, l - l1);
							o += l - l1;
						}
						buffer[o++] = '\n';
					}
				}
				if (o)
				{
					WriteFile((HANDLE)hfile, buffer, o, &numberofbytes, NULL);
					o = 0;
				}

				CloseHandle(hfile);
			}

			tickcount1 = GetTickCount();

			printf("Cost %d\r\n", tickcount1 - tickcount0);
		}
	}

	return(0);
}