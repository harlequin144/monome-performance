//Open Port

int USB = open( "/dev/ttyUSB0", O_RDWR| O_NOCTTY );
Set parameters

struct termios tty;
struct termios tty_old;
memset (&tty, 0, sizeof tty);

/* Error Handling */
if ( tcgetattr ( USB, &tty ) != 0 )
{
cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << endl;
}

/* Save old tty parameters */
tty_old = tty;

/* Set Baud Rate */
cfsetospeed (&tty, (speed_t)B9600);
cfsetispeed (&tty, (speed_t)B9600);

/* Setting other Port Stuff */
tty.c_cflag     &=  ~PARENB;        // Make 8n1
tty.c_cflag     &=  ~CSTOPB;
tty.c_cflag     &=  ~CSIZE;
tty.c_cflag     |=  CS8;

tty.c_cflag     &=  ~CRTSCTS;       // no flow control
tty.c_cc[VMIN]      =   1;                  // read doesn't block
tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

/* Make raw */
cfmakeraw(&tty);

/* Flush Port, then applies attributes */
tcflush( USB, TCIFLUSH );
if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
{
cout << "Error " << errno << " from tcsetattr" << endl;
}



//Write

unsigned char cmd[] = "INIT \r";
int n_written = 0;

do {
    n_written += write( USB, &cmd[n_written], 1 );
}
while (cmd[n_written-1] != '\r' && n_written > 0);
It was definitely not necessary to write byte per byte, also int n_written = write( USB, cmd, sizeof(cmd) -1) worked fine.

//At last, read:

int n = 0;
char buf = '\0';

/* Whole response*/
std::string response;

do
{
   n = read( USB, &buf, 1 );
   response.append( &buf );
}
while( buf != '\r' && n > 0);

if (n < 0)
{
   cout << "Error reading: " << strerror(errno) << endl;
}
   else if (n == 0)
{
    cout << "Read nothing!" << endl;
}
else
{
    cout << "Response: " << response;
}
This one worked for me. Thank you all!
