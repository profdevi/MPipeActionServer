/*    
    MPipeActionServer.exe : A Console Windows Pipe Server 
    Copyright (C) 2017  Comine.com

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

*/


//v1.2 copyright Comine.com 20170717M1205
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MWinNamedPipe.h"
#include "MProcessControl.h"
#include "MStringSplitter.h"
#include "MSystemLogoff.h"
#include "MLicenseGPL.h"
#include "MIntArray.h"
#include "MStringList.h"
#include "MWinProcessList.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MPipeActionServer";	// Used in Help
static const char *GApplicationVersion="1.2";	// Used in Help
static const char *GPipeName="\\\\.\\pipe\\MPipeActionServer";

////////////////////////////////////////////////////
static void GDisplayHelp(void);
static bool GProcessMessage(const char *mesg);
static bool GCloseApp(const char *appname);
static bool GKillApp(const char *procid);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	if(args.CheckRemoveArg("-gpl")==true)
		{
		MLicenseGPL license(true);
		license.Print();
		return 0;
		}

	if(args.CheckRemoveHelp()==true)
		{
		GDisplayHelp();
		return 0;
		}

	MWinNamedPipeServer server;
	if(server.Create(GPipeName)==false)
		{
		MStdPrintf("**Unable to start server for pipe %s\n",GPipeName);
		return 1;
		}

	MStdPrintf("*******************************************************************\n");
	MStdPrintf("**   %s Starting.....\n",GApplicationName);
	MStdPrintf("**   copyright Comine.com 2017\n");
	MStdPrintf("**   Listening on pipe %s\n",GPipeName);
	MStdPrintf("*******************************************************************\n");

	GDisplayHelp();

	for(;;)
		{
		if(server.Connect()==false)
			{
			MStdPrintf("**Connection Failed\n");
			continue;
			}

		char buf[100];
		const int msglen=server.Read(buf,sizeof(buf)-2);
		if(msglen<0)
			{
			MStdPrintf("**Message receive failed\n");
			return false;
			}

		buf[msglen]=0;
		MStdPrintf("Message Received: %s\n",buf);

		if(GProcessMessage(buf)==false)
			{
			MStdPrintf("**Message Not Executed\n");
			}

		server.Disconnect();
		}

	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-?|-gpl]\n"
				"           v%s copyright Comine.com\n"
				"           -gpl will display the Gnu Public License\n"
				"\n"
				"   Program accepts text message from a pipe and performs actions based\n"
				"   on the messages.  Use MPipeClient to send message to the pipe.\n"
				"\n"
				"   Commands:\n"
				"            pid               : show server process id\n"
				"            kill <pid>        : Kill a process\n"
				"            logout            : logout user \n"
				"            forcelogout       : forceably logout user\n"
				"            close <exename>   : send WM_QUIT Message to app\n"
				"\n"
				"   Example:\n"
				"             C:>  MPipeClient <pipename> \"pid\"\n"
				"\n"
				,GApplicationName,GApplicationVersion);
	}


////////////////////////////////////////////////////////////////
static bool GProcessMessage(const char *mesg)
	{
	MStringSplitter splitter;
	if(splitter.Create(mesg)==false)
		{
		MStdPrintf("**Unable to split message\n");		
		return false;
		}

	if(splitter.GetCount()<=0) { return false; }

	const char *cmd=splitter.Get(0);
	const int argcount=splitter.GetCount();

	// show pid
	if(MStdStrICmp(cmd,"pid")==0)
		{
		// Show Process ID
		MStdPrintf("Server Process ID: %u\n",MStdGetProcessID());
		return true;
		}

	// Logout 
	if(MStdStrICmp(cmd,"logout")==0)
		{
		MStdPrintf("About to logout of system\n");
		MSystemLogoff systemlogout(true);
		systemlogout.LogOff();
		return true;
		}


	// forcelogout
	if(MStdStrICmp(cmd,"forcelogout")==0)
		{
		MStdPrintf("About to force logout of system\n");
		MSystemLogoff systemlogout(true);
		systemlogout.LogOffForced();
		return true;
		}

	// ie close notepad.exe
	if(MStdStrICmp(cmd,"close")==0 && argcount>=2)
		{
		return GCloseApp(splitter.Get(1));
		}

	// ie close notepad.exe
	if(MStdStrICmp(cmd,"kill")==0 && argcount>=2)
		{
		return GKillApp(splitter.Get(1));
		}


	return true;
	}


///////////////////////////////////////////////////////////////
static bool GCloseApp(const char *appname)
	{
	MStdPrintf("Got Request to close App %s\n",appname);
	MWinProcessList proclist;
	if(proclist.Create()==false)
		{
		MStdPrintf("**Unable to init system\n");
		return false;
		}

	const int index=proclist.FindProcessIndex(appname);
	if(index<0)
		{
		MStdPrintf("**Unable to find process %s\n",appname);
		return false;
		}

	const int pid=proclist.GetProcessID(index);

	MProcessControl proccontrol(true);
	MStdPrintf("Sending WM_CLOSE message to process...\n");
	proccontrol.Close(pid);

	return true;
	}


//////////////////////////////////////////////////////////////
static bool GKillApp(const char *procid)
	{
	MStdPrintf("Got Request to close App %s\n",procid);

	const int processid=MStdAToI(procid);
	if(processid<=0)
		{
		MStdPrintf("**Bad Process ID passed to kill\n");
		return false;
		}

	MProcessControl proccontrol(true);
	MStdPrintf("Killing Process with Process ID %d\n",processid);
	proccontrol.Kill(processid);
	return true;
	}

