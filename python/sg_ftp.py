from ftplib import FTP
import sys
import os

def sg_ftp(filename, directory="/user/bbg1/waveform"):

    filename_stripped = os.path.basename(filename)
    ftp = FTP(host='134.117.62.53', user='', passwd='')
    print(ftp.login())
    ftp.cwd('/user/bbg1/waveform')
    ftp.storbinary('STOR '+ filename_stripped, open(filename, 'rb'))
    print(ftp.nlst())
    print(ftp.quit())

if __name__ == '__main__':
    sg_ftp(sys.argv[1])
