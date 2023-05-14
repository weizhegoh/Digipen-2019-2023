-- Settings

settings = {}
settings["workspace"] = "Sundown's Epiphany"
settings["outputdir"] = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}-%{cfg.platform}"
settings["targetdir"] = "%{wks.location}/bin/%{settings.outputdir}"
settings["objdir"] = "%{wks.location}/bin-int/%{settings.outputdir}"