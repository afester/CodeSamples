'''
Created on 23.02.2015

@author: afester
'''

import dropbox
import configparser
import time

from dropbox.datastore import DatastoreError, DatastoreManager, Date, Bytes


class DropShell:
    
    def __init__(self):
        self.config = configparser.ConfigParser()
        self.config.read('dropshell.ini')
        self.settings = self.config["DropBox"]
        self.wdPath = []
        self.datastoreManager = None

    def getFilePath(self, fileName):
        if len(self.wdPath) == 0:
            return '/' + fileName
        return '/' + '/'.join(self.wdPath) + '/' + fileName


    def appAuthorization(self):
        print('Application authorization required')
        print('---------------------------------------------------------\n')

        app_key = self.settings['AppKey']
        app_secret= self.settings['AppKeySecret']

        flow = dropbox.client.DropboxOAuth2FlowNoRedirect(app_key, app_secret)
        authorize_url = flow.start()

        print('   1. Go to: ' + authorize_url)
        # "AppName" moechte auf ihren eigenen Ordner Apps > NotesWiki in Ihrer Dropbox zugreifen.
        # Allow, Deny 

        print('   2. Click "Allow" (you might have to log in first)')

        print('   3. Copy the authorization code.\n')

        code = input("Enter the authorization code here: ").strip()
        access_token, user_id = flow.finish(code)

        # "The access token is all you'll need to make API requests on behalf 
        #  of this user, so you should store it away for safe-keeping. 
        #  By storing the access token, you won't need to go through these steps 
        #  again unless the user reinstalls your app or revokes access via the 
        #  Dropbox website."

        self.settings['UserAccessToken'] = access_token
        self.settings['UserAccessId'] = user_id

        with open('dropshell.ini', 'w') as dropshellIni:
            self.config.write(dropshellIni, False)


    def processCommand(self, tokens):
        cmd = tokens[0].lower()
        if cmd == "quit":
            self.isRunning = False
        elif cmd == "connect":
            self.connect(tokens[1:])
        elif cmd == "ls":
            self.listFiles(tokens[1:])
        elif cmd == "cd":
            self.changeDirectory(tokens[1:])
        elif cmd == "info":
            self.showAccountInfo()
        elif cmd == "put":
            self.putFile(tokens[1:])
        elif cmd == "get":
            self.getFile(tokens[1:])
        elif cmd == "rm":
            self.deleteFile(tokens[1:])
        elif cmd == "mkdir":
            self.mkdir(tokens[1:])
        elif cmd == "rmdir":
            self.rmdir(tokens[1:])
        elif cmd == "help":
            self.showHelp()
        elif cmd == "dscreate":
            self.dsCreate(tokens[1:])
        elif cmd == "dslist":
            self.dsList()
        else:
            print("Syntax error: {}".format(tokens))


    def showHelp(self):
        print('''
Available commands:
-------------------
  help              - Show this help summary

  connect           - Connect to dropbox
  info              - Show information about the connection

  ls [-l]           - Show files and folders in the current folder

  cd path           - Change the current path 
  mkdir folderName  - Creates a new folder in the current folder
  put fileName      - Upload a file to the current folder
  get fileName      - Download a file from the current folder
  rm name           - Deletes the specified file or folder from the current folder

  ds                - execute a Datastore API sample

  quit              - Leave the command line client''')

    def connect(self, parameters):
        if self.settings.get('useraccesstoken') is None:
            self.appAuthorization()

        self.client = dropbox.client.DropboxClient(self.settings['useraccesstoken'])
        print("Connected.")
        self.showAccountInfo()


    def showAccountInfo(self):
        info = self.client.account_info()
        
        print("Connection information")
        print("-------------------------------")
        print('  User : {}'.format(info['display_name']))
        print('  eMail: {}'.format(info['email']))
        print()
        


    def listFiles(self, parameters):
        isLong = len(parameters) > 0 and parameters[0] == '-l'

        # Depending on the application type / security settings, the
        # root directory "/" either points to the applications root directory
        # (e.g. DropBox > Apps > NotesWiki) or to the User's root directory
        # ("DropBox")

        wd = '/' + '/'.join(self.wdPath)
        contents = self.client.metadata(wd)
        files = contents['contents']

        print()
        for f in files:
            filePath = f['path'].split('/')
            fileName = filePath[-1]
            isDir = f['is_dir']

            if isLong:
                # Www, DD Mmm YYYY HH:MM:SS +Tz
                mDate = time.strptime(f['modified'], "%a, %d %b %Y %H:%M:%S %z")
                printDate = time.strftime("%b %d %Y %H:%M:%S", mDate)
                size = f['size']
                print('{:>10}  {}  {}{}'.format(size, printDate, fileName, '/' if isDir else ''))
            else:
                print('{}{}'.format(fileName, '/' if isDir else ''))


    def changeDirectory(self, parameters):
        dirName = parameters[0]
        if dirName == '..':
            if len(self.wdPath) > 0:
                self.wdPath = self.wdPath[:-1]
        else:
            self.wdPath.append(dirName)


    def putFile(self, params):
        remoteFileName = self.getFilePath(params[0])
        print("Uploading: {} => {}".format(params[0], remoteFileName))
        f = open(params[0], 'rb')
        response = self.client.put_file(remoteFileName, f)
        print("Uploaded {} bytes.".format(response['size']))

        #f = open('working-draft.txt', 'rb')
        #response = client.put_file('/magnum-opus.txt', f)
        #print "uploaded:", response


    def getFile(self, params):
        remoteFileName = self.getFilePath(params[0])
        print("Downloading: {} => {}".format(remoteFileName, params[0]))
        out = open(params[0], 'wb')
        with self.client.get_file(remoteFileName) as f:
            out.write(f.read())
        print("Done.")


    def mkdir(self, params):
        remoteDirName = self.getFilePath(params[0])
        result = self.client.file_create_folder(remoteDirName)
        print("Created directory {}".format(result['path']))


    #def rmdir(self, params):
    #    remoteDirName = self.getFilePath(params[0])
    #    result = self.client.file_delete(remoteDirName)
    #    print("Removed directory {}".format(result['path']))


    def deleteFile(self, params):
        remoteFileName = self.getFilePath(params[0])
        response = self.client.file_delete(remoteFileName)
        print("Removed {} {}".format("directory" if response['is_dir'] else "file", response['path']))


# Datastore API samples

    def dsCreate(self, params):
        if self.datastoreManager is None:
            self.datastoreManager = DatastoreManager(self.client)
        result = self.datastoreManager.open_or_create_datastore(params[0])
        print(str(result))


    def dsDelete(self, params):
        if self.datastoreManager is None:
            self.datastoreManager = DatastoreManager(self.client)
        result = self.datastoreManager.delete_datastore(params[0], True)
        print(str(result))


    def dsList(self):
        if self.datastoreManager is None:
            self.datastoreManager = DatastoreManager(self.client)
        result = self.datastoreManager.list_datastores()
        for ds in result:
            print(str(ds))


    def processInput(self):
        self.connect(None)

        self.isRunning = True
        while self.isRunning:
            wd = '/' + '/'.join(self.wdPath)
            s = input('[DS] {} > '.format(wd))

            tokens = s.split(maxsplit=1)
            if len(tokens) > 0:
                self.processCommand(tokens)
            print()


if __name__ == '__main__':
    d = DropShell()
    d.processInput()
