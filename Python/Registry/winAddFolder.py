#!/usr/bin/python

import sys
import os
import uuid

import ctypes
import winreg
import winerror


REG_TYPES = {winreg.REG_BINARY: 'REG_BINARY',
             winreg.REG_DWORD: 'REG_DWORD',
             winreg.REG_DWORD_LITTLE_ENDIAN: 'REG_DWORD_LITTLE_ENDIAN',
             winreg.REG_DWORD_BIG_ENDIAN: 'REG_DWORD_BIG_ENDIAN',
             winreg.REG_EXPAND_SZ: 'REG_EXPAND_SZ',
             winreg.REG_LINK: 'REG_LINK',
             winreg.REG_MULTI_SZ: 'REG_MULTI_SZ',
             winreg.REG_NONE: 'REG_NONE',
             winreg.REG_QWORD: 'REG_QWORD',
             winreg.REG_QWORD_LITTLE_ENDIAN: 'REG_QWORD_LITTLE_ENDIAN',
             winreg.REG_RESOURCE_LIST: 'REG_RESOURCE_LIST',
             winreg.REG_FULL_RESOURCE_DESCRIPTOR: 'REG_FULL_RESOURCE_DESCRIPTOR',
             winreg.REG_RESOURCE_REQUIREMENTS_LIST: 'REG_RESOURCE_REQUIREMENTS_LIST',
             winreg.REG_SZ: 'REG_SZ'}


def isAdmin():
    try:
        is_admin = (os.getuid() == 0)   # Linux case
    except AttributeError:
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
    return is_admin


def regTypeToStr(typeId):
    typeStr = REG_TYPES.get(typeId)
    if typeStr is None:
        typeStr = 'REG_???'
    return typeStr


def getSubKeys(key):
    subKeys = list()
    try:
        idx = 0
        while True:
            subKey = winreg.EnumKey(key, idx)
            idx += 1
            subKeys.append(subKey)
    except WindowsError as oerr:
        if oerr.winerror != winerror.ERROR_NO_MORE_ITEMS:
            raise

    return subKeys


def getValues(key):
    values = list()
    try:
        idx = 0
        while True:
            value = winreg.EnumValue(key, idx)
            idx += 1
            values.append(value)
    except WindowsError as oerr:
        if oerr.winerror != winerror.ERROR_NO_MORE_ITEMS:
            raise

    return values


def getValuesMap(key):
    result = dict()
    try:
        idx = 0
        while True:
            value = winreg.EnumValue(key, idx)
            idx += 1

            valueName = value[0]
            if not valueName:
                valueName = '(Default)'
            valueType = value[2]
            valueData = value[1]

            result[valueName] = (valueType, valueData)
    except WindowsError as oerr:
        if oerr.winerror != winerror.ERROR_NO_MORE_ITEMS:
            raise

    return result


def getEntriesForPath(pathName):
    keyObject = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, pathName)
    subKeys = getSubKeys(keyObject)
    values = getValues(keyObject)
    winreg.CloseKey(keyObject)
    return subKeys, values


def getKnownFolder(folderId):
    keyObject = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,
                               'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FolderDescriptions\\'
                               + folderId)
    values = getValuesMap(keyObject)
    winreg.CloseKey(keyObject)

    folderName = values.get('Name')
    if folderName is not None:
        folderName = folderName[1]
    folderPath = values.get('RelativePath')
    if folderPath is not None:
        folderPath = folderPath[1]
    return dict(id=folderId, name=folderName, path=folderPath)


def dumpRegTree(indentLevel, pathName):
    subKeys, values = getEntriesForPath(pathName)
    for value in values:
        valueName = value[0]
        if not valueName:
            valueName = '(Default)'
        valueType = value[2]
        valueData = value[1]

        print('{}{} ({}): {}'.format('   ' * indentLevel, valueName, regTypeToStr(valueType), valueData))
        if valueName == 'TargetKnownFolder':
            getKnownFolder(valueData)
    for subKey in subKeys:
        print('   ' * indentLevel + subKey)
        dumpRegTree(indentLevel + 1, pathName + '\\' + subKey)


def getKnownFolders():
    keyObject = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,
                               'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FolderDescriptions')
    knownFolders = getSubKeys(keyObject)
    winreg.CloseKey(keyObject)

    result = list()
    for folderId in knownFolders:
        folder = getKnownFolder(folderId)
        result.append(folder)
    result.sort(key=lambda k: k['name'])
    return result


def listKnownFolders():
    folderList = getKnownFolders()
    for folder in folderList:
        print(folder)


def listMyComputerFolders():
    myComputerNameSpaceKey = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,
                                            'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\' +
                                            'Explorer\\MyComputer\\NameSpace')
    subKeys = getSubKeys(myComputerNameSpaceKey)
    for key in subKeys:
        print(key)
        if key.startswith('{'):
            classIdRegPath = 'SOFTWARE\\Classes\\CLSID\\' + key
            dumpRegTree(1, classIdRegPath)
    winreg.CloseKey(myComputerNameSpaceKey)


def createFolderClass(pathName):
    """
    Creates a new class id for the given path name.

    :param pathName: The path name of the new folder to add.
    """

    # key [HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{f86fa3ab-70d2-4fc7-9c99-fcbf05467f3a}]
    folderClassId = uuid.uuid4()
    folderKeyName = '{' + str(folderClassId) + '}'
    print('Creating class {}'.format(folderKeyName))
    parentKey = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE,
                               'SOFTWARE\\Classes\\CLSID')
    folderKey = winreg.CreateKey(parentKey, folderKeyName)
    winreg.CloseKey(parentKey)

    winreg.SetValueEx(folderKey, 'DescriptionID', 0, winreg.REG_DWORD_LITTLE_ENDIAN, '3')
    winreg.SetValueEx(folderKey, 'Infotip', 0, winreg.REG_SZ, 'My Projects')
    winreg.SetValueEx(folderKey, 'System.IsPinnedToNameSpaceTree', 0, winreg.REG_DWORD_LITTLE_ENDIAN, '1')

    # key [HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{f86fa3ab-70d2-4fc7-9c99-fcbf05467f3a}\DefaultIcon]
    defaultIconKey = winreg.CreateKey(folderKey, 'DefaultIcon')
    winreg.SetValueEx(defaultIconKey, '', 0, winreg.REG_EXPAND_SZ, '%SystemRoot%\\system32\\imageres.dll,-113')
    winreg.CloseKey(defaultIconKey)

    # key [HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{f86fa3ab-70d2-4fc7-9c99-fcbf05467f3a}\InProcServer32]
    inProcServer32Key = winreg.CreateKey(folderKey, 'InProcServer32')
    winreg.SetValueEx(inProcServer32Key, '', 0, winreg.REG_EXPAND_SZ, '%systemroot%\\system32\\shell32.dll')
    winreg.SetValueEx(inProcServer32Key, 'ThreadingModel', 0, winreg.REG_SZ, 'Both')
    winreg.CloseKey(inProcServer32Key)

    # key [HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{f86fa3ab-70d2-4fc7-9c99-fcbf05467f3a}\Instance]
    instanceKey = winreg.CreateKey(folderKey, 'Instance')
    # "CLSID"="{0E5AAE11-A475-4c5b-AB00-C66DE400274E}"

    # key [HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{f86fa3ab-70d2-4fc7-9c99-fcbf05467f3a}\Instance\InitPropertyBag]
    instanceInitPropertyBagKey = winreg.CreateKey(instanceKey, 'InitPropertyBag')
    winreg.SetValueEx(instanceInitPropertyBagKey, 'Attributes', 0, winreg.REG_DWORD_LITTLE_ENDIAN, '17')
    # "TargetKnownFolder"="{35286a68-3c57-41a1-bbb1-0eae73d76c95}"
    winreg.SetValueEx(instanceInitPropertyBagKey, 'TargetFolderPath', 0, winreg.REG_SZ, pathName)

    winreg.CloseKey(instanceInitPropertyBagKey)
    winreg.CloseKey(instanceKey)

    # key [HKEY_LOCAL_MACHINE\SOFTWARE\Classes\CLSID\{f86fa3ab-70d2-4fc7-9c99-fcbf05467f3a}\ShellFolder]
    shellFolderKey = winreg.CreateKey(folderKey, 'InitPropertyBag')
    winreg.SetValueEx(shellFolderKey, 'Attributes', 0, winreg.REG_DWORD_LITTLE_ENDIAN, '4034920525')
    winreg.SetValueEx(shellFolderKey, 'FolderValueFlags', 0, winreg.REG_DWORD_LITTLE_ENDIAN, '41')
    winreg.SetValueEx(shellFolderKey, 'SortOrderIndex', 0, winreg.REG_DWORD_LITTLE_ENDIAN, '0')
    winreg.CloseKey(shellFolderKey)

    winreg.CloseKey(folderKey)


def addMyComputerFolder(pathName):
    print('Adding ' + pathName)

    createFolderClass(pathName)


def usage():
    print('Usage: winAddFolder <command>')
    print('where <command> is one of')
    print('    listKnownFolders')
    print('    listMyComputerFolders')


def main(argv):
    if len(argv) <= 1:
        usage()
        sys.exit()

    if not isAdmin():
        print('This program must be run as Administrator!')
        sys.exit()

    if argv[1] == 'listKnownFolders':
        listKnownFolders()
    elif argv[1] == 'listMyComputerFolders':
        listMyComputerFolders()
    elif len(argv) == 3 and argv[1] == 'addMyComputerFolder':
        addMyComputerFolder(argv[2])
    else:
        usage()


if __name__ == '__main__':
    main(sys.argv)
