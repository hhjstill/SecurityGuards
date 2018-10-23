#pragma once
#include <iostream>
using namespace std;
#include <windows.h>
#include <vector>
#include <atlstr.h>

struct SoftInfo
{
	// 软件名
	CString m_strSoftName;
	// 软件版本号
	CString m_strSoftVersion;
	// 软件安装目录
	CString m_strInstallLocation;
	// 软件发布厂商
	CString m_strPublisher;
	// 主程序所在完整路径
	CString m_strMainProPath;
	// 卸载exe所在完整路径
	CString m_strUninstallPth;
};

class CSoftInfo
{
public:
	// 保存已安装常用软件安装信息
	std::vector<SoftInfo> m_SoftInfoArr;
	// 保存系统补丁信息
	std::vector<SoftInfo> m_SystemPatchesArr;

public:
	CSoftInfo();
	~CSoftInfo() {}

	// 获取一个包含常用软件安装信息的Vector
	std::vector<SoftInfo> GetSoftInfo(void) const;
	// 获取所有已安装常用软件名
	void GetSoftName(std::vector<LPCTSTR>& lpszSoftName);
	// 获取所有已安装常用软件版本号
	void GetSoftVersion(std::vector<LPCTSTR>& lpszSoftVersion);
	// 获取所有已安装常用软件安装目录
	void GetInstallLocation(std::vector<LPCTSTR>& lpszInstallLocation);
	// 获取所有已安装常用软件发布厂商
	void GetPublisher(std::vector<LPCTSTR>& lpszPublisher);
	// 获取所有已安装常用软件主程序所在路径
	void GetMainProPath(std::vector<LPCTSTR>& lpszMainProPath);
	// 获取所有已安装常用软件卸载程序所在路径
	void GetUninstallPth(std::vector<LPCTSTR>& lpszSoftName);

	// 获取一个包含系统补丁信息的Vector
	std::vector<SoftInfo> GetSystemPatchesInfo(void) const;
	// 获取所有已安装系统补丁名
	void GetSystemPatchesName(std::vector<LPCTSTR>& lpszSoftName);
};