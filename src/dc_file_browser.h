#ifndef _DC_FILE_BROWSER_H_
#define _DC_FILE_BROWSER_H_

#include <kos.h>
#include "dc_menu.h"
#include "XArray.h"

class DCFileBrowser
{
public:
	DCFileBrowser();
	virtual ~DCFileBrowser();
	
	bool Browse(bool use_last = false);
	void SetDirectory(const char* szdir);
	const char* GetDirectory();
	void SetHighestDirecty(const char* szdir);
	const char* GetHighestDirecty();
	const char* GetChosenFilePath();
	void SetDefaultIcon(CPVRTexture* icon) { m_default_icon = icon; }
	void SetFolderIcon(CPVRTexture* icon) { m_folder_icon = icon; }
	void SetIcon(CPVRTexture* icon, const char* extension);

	DCMenu* GetMenu() { return &m_menu; }
	
	static void OnChooseDir(DCMenu* pMenu, DCMenuItem* pMenuItem, int value);
	static void OnChooseFile(DCMenu* pMenu, DCMenuItem* pMenuItem, int value);
	
	uint32 directory_color;

protected:
	uint8 m_deep;
	bool m_found;
	DCMenu m_menu;
	char m_dir[0x1000];
	char m_highest_dir[0x1000];
	char m_file_path[0x1000];
	bool m_has_old_data;
	CPVRTexture* m_default_icon;
	CPVRTexture* m_folder_icon;
	XArray<CPVRTexture*> m_icons;
	XArray<char*> m_icon_extensions;
};

#endif

