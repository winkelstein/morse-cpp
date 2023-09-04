#pragma once

#ifdef _WIN32
#include <Windows.h>
#else					  /* UNIX */
#define _POSIX_C_SOURCE 2 /* to accept POSIX 2 in old ANSI C standards */
#include <string.h>
#include <dirent.h> /* on old systems try <sys/dir.h> instead */
#include <errno.h>
#include <signal.h> /* on old systems try <sys/signal.h> instead */
#endif
#include <stdio.h>

char beep_version[8] = "1.0.9"; /* contains beeps current version number */

int beep_verbose = 0; /* 0 (default) or 1 : prints the command line calls */

#ifdef _WIN32
static int powershellPresent(void)
{
	static int lPowershellPresent = -1;

	if (lPowershellPresent < 0)
	{
		char lBuff[MAX_PATH];
		FILE *lIn;
		char const *lString = "powershell.exe";
		if (!(lIn = _popen("where powershell.exe", "r")))
		{
			lPowershellPresent = 0;
			return 0;
		}
		while (fgets(lBuff, sizeof(lBuff), lIn) != NULL)
		{
		}
		_pclose(lIn);
		if (lBuff[strlen(lBuff) - 1] == '\n')
		{
			lBuff[strlen(lBuff) - 1] = '\0';
		}
		if (strcmp(lBuff + strlen(lBuff) - strlen(lString), lString))
		{
			lPowershellPresent = 0;
		}
		else
		{
			lPowershellPresent = 1;
		}
	}
	return lPowershellPresent;
}

static int windowsVersion(void)
{
#if !defined(__MINGW32__) || defined(__MINGW64_VERSION_MAJOR)
	typedef LONG NTSTATUS;
	typedef NTSTATUS(WINAPI * RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
	HMODULE hMod;
	RtlGetVersionPtr lFxPtr;
	RTL_OSVERSIONINFOW lRovi = {0};

	hMod = GetModuleHandleW(L"ntdll.dll");
	if (hMod)
	{
		lFxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (lFxPtr)
		{
			lRovi.dwOSVersionInfoSize = sizeof(lRovi);
			if (!lFxPtr(&lRovi))
			{
				return lRovi.dwMajorVersion;
			}
		}
	}
#endif
	if (powershellPresent())
		return 6; /*minimum is vista or installed on xp*/
	return 0;
}
#else
static void sigHandler(int sig)
{
	FILE *lIn;
	if ((lIn = popen("pactl unload-module module-sine", "r")))
	{
		pclose(lIn);
	}
}

static int detectPresence(char const *aExecutable)
{
	char lBuff[256];
	char lTestedString[256] = "which ";
	FILE *lIn;

	strcat(lTestedString, aExecutable);
	strcat(lTestedString, " 2>/dev/null ");
	lIn = popen(lTestedString, "r");
	if ((fgets(lBuff, sizeof(lBuff), lIn) != NULL) && (!strchr(lBuff, ':')) && (strncmp(lBuff, "no ", 3)))
	{ /* present */
		pclose(lIn);
		if (beep_verbose)
			printf("detectPresence %s %d\n", aExecutable, 1);
		return 1;
	}
	else
	{
		pclose(lIn);
		if (beep_verbose)
			printf("detectPresence %s %d\n", aExecutable, 0);
		return 0;
	}
}

static int osascriptPresent()
{
	static int lOsascriptPresent = -1;
	if (lOsascriptPresent < 0)
	{
		lOsascriptPresent = detectPresence("osascript");
	}
	return lOsascriptPresent;
}

static int afplayPresent()
{
	static int lAfplayPresent = -1;

	if (lAfplayPresent < 0)
	{
		lAfplayPresent = detectPresence("afplay");
		if (lAfplayPresent)
		{
			char lBuff[256];
			FILE *lIn = popen("test -e /System/Library/Sounds/Ping.aiff || echo Ping", "r");
			if (fgets(lBuff, sizeof(lBuff), lIn) == NULL)
			{
				lAfplayPresent = 2;
			}
			pclose(lIn);
			if (beep_verbose)
				printf("afplay %d\n", lAfplayPresent);
		}
	}
	return lAfplayPresent;
}

static int pactlPresent()
{
	static int lPactlPresent = -1;
	if (lPactlPresent < 0)
	{
		lPactlPresent = detectPresence("pactl");
	}
	return lPactlPresent;
}

static int speakertestPresent()
{
	static int lSpeakertestPresent = -1;
	if (lSpeakertestPresent < 0)
	{
		lSpeakertestPresent = detectPresence("speaker-test");
	}
	return lSpeakertestPresent;
}

static int beepexePresent()
{
	static int lBeepexePresent = -1;
	if (lBeepexePresent < 0)
	{
		lBeepexePresent = detectPresence("beep.exe");
	}
	return lBeepexePresent;
}

static int beepPresent()
{
	static int lBeepPresent = -1;
	if (lBeepPresent < 0)
	{
		lBeepPresent = detectPresence("beep");
	}
	return lBeepPresent;
}

static int playPresent()
{
	static int lPlayPresent = -1;
	if (lPlayPresent < 0)
	{
		lPlayPresent = detectPresence("sox");
	}
	return lPlayPresent;
}

#endif /* UNIX */

void beep(float aFrequency_Hz, int aLength_ms)
{
#ifndef _WIN32
	char lDialogString[256];
	FILE *lIn;
	DIR *lDir;
#endif

	aFrequency_Hz = aFrequency_Hz > 0 ? aFrequency_Hz : 440.f;
	aLength_ms = aLength_ms > 0 ? aLength_ms : 400;

#ifdef _WIN32
	if (beep_verbose)
		printf("windows Beep %dHz %dms\n", (int)aFrequency_Hz, aLength_ms);
	if (windowsVersion() > 5)
		Beep((DWORD)aFrequency_Hz, aLength_ms);
	else
		MessageBeep(MB_OK);
#else /* UNIX */

	if (pactlPresent())
	{
		signal(SIGINT, sigHandler);

		/*strcpy( lDialogString , "pactl load-module module-sine frequency=440;sleep .3;pactl unload-module module-sine" ) ;*/
		sprintf(lDialogString,
				"thnum=$(pactl load-module module-sine frequency=%d);sleep %f;pactl unload-module $thnum",
				(int)aFrequency_Hz, aLength_ms / 1000.f);
	}
	else if (speakertestPresent())
	{
		/*strcpy( lDialogString , "timeout -k .3 .3 speaker-test --frequency 440 --test sine > /dev/tty" ) ;*/
		lDir = opendir("/dev/tty");
		if (!lDir && (ENOENT != errno))
		{
			sprintf(lDialogString,
					"(speaker-test -t sine -f %f > /dev/tty) & pid=$!;sleep %f;kill -9 $pid",
					aFrequency_Hz, aLength_ms / 1000.f);
		}
		else
		{
			sprintf(lDialogString,
					"(speaker-test -t sine -f %f) & pid=$!;sleep %f;kill -9 $pid",
					aFrequency_Hz, aLength_ms / 1000.f);
		}
	}
	else if (beepexePresent())
	{
		sprintf(lDialogString, "beep.exe %f %d\n", aFrequency_Hz, aLength_ms);
	}
	else if (playPresent()) /* play is part of sox */
	{
		sprintf(lDialogString, "play -n synth %f sine %f\n", aLength_ms / 1000.f, aFrequency_Hz);
	}
	else if (beepPresent())
	{
		sprintf(lDialogString, "beep -f %f -l %d\n", aFrequency_Hz, aLength_ms);
	}
	else if (osascriptPresent())
	{
		if (afplayPresent() >= 2)
		{
			strcpy(lDialogString, "afplay /System/Library/Sounds/Ping.aiff");
		}
		else
		{
			strcpy(lDialogString, "osascript -e 'tell application \"System Events\" to beep'");
		}
	}
	else
	{
		lDir = opendir("/dev/tty");
		if (!lDir && (ENOENT != errno))
			strcpy(lDialogString, "printf '\a' > /dev/tty");
		else
			strcpy(lDialogString, "printf '\a'");
	}

	if (beep_verbose)
		printf("beep-cmdline: %s\n", lDialogString);

	if ((lIn = popen(lDialogString, "r")))
	{
		pclose(lIn);
	}

	if (pactlPresent())
	{
		signal(SIGINT, SIG_DFL);
	}

#endif /* UNIX */
}