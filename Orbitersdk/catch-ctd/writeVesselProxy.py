import sys
import CppHeaderParser
import re
try:
    cppHeader = CppHeaderParser.CppHeader("..\\include\\VesselAPI.h.edited")
except CppHeaderParser.CppParseError as e:
    print(e)
    sys.exit(1)
header = """\
#ifndef CATCH_CTD_VESSEL_PROXY
#define CATCH_CTD_VESSEL_PROXY
#include "Orbitersdk.h"

"""
cpp = """\
#include "VesselProxy.h"
#include "Log.h"

"""

#foreach vessel class
for x in range(1,4):
	className = "VESSEL"
	if x != 1:
		className += str(x)
	vesselClass = cppHeader.classes[className]
	proxyClassName = className + "Proxy"
	
	#now generate the proxy class start
	header += ("class " + proxyClassName + " : public " + className + "\n{\npublic:\n" 
		+ "	" + proxyClassName + "(" + className + "* pVessel, void(*_realOvcExit)(VESSEL*), OBJHANDLE hVessel, int fmodel);" + "\n" 
		+ "	~" + proxyClassName + "();" + "\n" )
		
	#generate the constructor/deconstructor of our vessel
	cpp += (proxyClassName + "::" + proxyClassName + "(" + className + "* pVessel, void(*_realOvcExit)(VESSEL*), OBJHANDLE hVessel, int fmodel) : "
		+ className + "(hVessel,fmodel)\n{\n"
		+ "	realOvcExit = _realOvcExit;\n	vesselName = pVessel->GetName();\n	className = pVessel->GetClassName();\n	actualVessel = pVessel;\n	Log::writeToLogDameon(IndentString(), \"Inside \", className, \"::\", vesselName, \"::constructor\\r\\n\");\n}\n")
	cpp += (proxyClassName + "::~" + proxyClassName + "()\n{\n	Log::writeToLogDameon(IndentString(), \"Inside \", className, \"::\", vesselName, \"::deconstructor\\r\\n\");\n	realOvcExit(actualVessel);\n}\n")
		
	#Ignore the first method, we have our own constructor
	for method in vesselClass["methods"]["public"][1:]:
		#ignore deprecated Create call
		if method["name"] == "Create":
			continue
		fullParamList = []
		paramNameList = []
		for parameter in method["parameters"]:
			fullParamList.append(parameter["type"] + " " + parameter["name"])
			paramNameList.append(parameter["name"])
		header += ("\t" + method["rtnType"] + " " + method["name"] + "(" + ", ".join(fullParamList) + ");\n")
		
		cpp += (method["rtnType"] + " " + proxyClassName + "::" + method["name"] + "(" + ", ".join(fullParamList) + ")\n{\n"
			+ "	Log::writeToLogDameon(IndentString(),\"Inside \", className, \"::\", vesselName, \"::" + method["name"] +":\" ")
		for argName in paramNameList:
			if argName != paramNameList[0]:
				cpp += ", \", "
			else:
				cpp += ", \""
			cpp += (argName + ":\", " + argName)
		cpp += (",\"\\r\\n\");" + "\n	Log::increaseIndent();\n");
		if (method["rtnType"] == "void"):
			cpp += ("	actualVessel->" + method["name"] + "(" + ", ".join(paramNameList) + ");"
				+ "\n	Log::decreaseIndent();\n	Log::writeToLogDameon(\"...\\r\\n\");\n}\n")
		else:
			cpp += ("	" + method["rtnType"] + " result = actualVessel->" + method["name"] + "(" + ", ".join(paramNameList) + ");"
				+ "\n	Log::decreaseIndent();\n	Log::writeToLogDameon(\"...\\r\\n\");" + "\n	return result;\n}\n")
	#generate ending code
	header += ("private:\n	" + className + "* actualVessel;\n	void(*realOvcExit)(VESSEL*);\n	char* vesselName;\n	char* className;\n};\n")
	
#and generate really final header code
header +="#endif\n"

#cleanup some stuff
#fixup bad parsing of UINT anim =(UINT) -1
header = header.replace("UINT anim =(UINT) -)", "UINT anim =(UINT) -1)")
cpp = cpp.replace("UINT anim =(UINT) -)", "UINT anim)")
cpp = cpp.replace(", -",", anim")
#fixup some void functions emitting , , into log call
cpp = cpp.replace(", ,", ", ")
#fixup inline and static coming along for the ride on return type
cpp = cpp.replace("inline int result", "int result")
cpp = cpp.replace("static OBJHANDLE result", "OBJHANDLE result")


headerFile = open("VesselProxy.h", 'w')
cppFile = open("VesselProxy.cpp", 'w')
headerFile.write(header)
cppFile.write(cpp)
headerFile.close()
cppFile.close()