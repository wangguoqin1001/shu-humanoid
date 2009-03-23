/*
 * Copyright (C) 2007 Wang Guoqin <wangguoqin1001@gmail.com>
 * May be copied or modified under the terms of the GNU General Public License.
 *
 * Some inclusions, definitions, structures, varibles, and declarations for functions.
 * Only for My_Pickup.c right now.
 */

#ifndef _Common_H_
#define _Common_H_

#ifdef HAS_GTK
#include <gtk/gtk.h>
#endif

#define MAX_POINTS_PER_COLOR 20

#define COLOR_TYPES 4

#define COLOR_RED 0
#define COLOR_GREEN 1
#define COLOR_BLUE 2
#define COLOR_YELLOW 3
#define COLOR_WHITE 4
#define COLOR_BLACK 5

static const char COLOR_NAME[][10] = {"red", "green", "blue", "yellow", "white", "black"};
//static const int COLOR_VALUE_RGB[][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {255, 255, 255}, {0, 0, 0}};


#define CAPTURE_WIDTH 640
#define CAPTURE_HEIGHT 480
#define CAPTURE_BPP 24
#define FRAME_PER_SECOND 10


#define SOCKET_IDS 6
#define ID_MASK 0x000F

#define SOCKET_LISTENER_ID 0x0000
#define VISIOND_ID 0x0001
#define CONSOLE_GUARDER_ID 0x0002
#define GTK_GUARDER_ID 0x0003
#define GTK_GUARDER_FRAME_ID 0x0004
#define MOTORD_ID 0x0005

#define NEED_FRAME 0x0010
#define DO_SEARCHING 0x0020

#define DATAGRAM_ID_MASK 0x3F00

#define LARGEST_DATAGRAM 14400

static const char SOCKET_ID[][20] = {"Socket Listener", "Visiond", "Console Guarder", "Gtk Guarder", "Gtk Guarder (Frame)"};
static const int MAX_CLIENTS[] = {1, 1, 10, 10, 10};


#define MOTOR_NUM 24


int Index_Coordinate[CAPTURE_WIDTH * CAPTURE_HEIGHT], Index_Number[CAPTURE_WIDTH * CAPTURE_HEIGHT], Index_Length;
unsigned char *frame;

struct HSVColor
{
	int H;
	int S;
	int V;
}; /* a set of HSV values, of one point */

struct HSVColors;
struct ColorIdentifier;

struct SearchResult
{
	int area;
	int aver_x;
	int aver_y;
} result[COLOR_TYPES];

struct PointMatched
{
	int capable;
	int color;
	int deviation_H;
	int deviation_S;
	int deviation_V;
};

struct FrameQueue;

struct VideoInfo
{
	int fps;
	float spf;
	int area[COLOR_TYPES];
	int aver_x[COLOR_TYPES];
	int aver_y[COLOR_TYPES];
};

struct motor_step
{
	unsigned char onestep[MOTOR_NUM];
};

struct motor_motion
{
	short step_index;
	short motion_steps;
	struct motor_step *data;
};

struct motor_response
{
	unsigned short runmode;
	unsigned short retcode;
};

static const struct motor_step step_init = {{82,139,82,27,77,57,86,11,109,104,89,99,180,0,0,0,0,0,0,0,0,0,0,0}};

/* in Visiond.c */
int SearchForColor(unsigned char *, struct FrameQueue *, struct FrameQueue *);

/* in Scatter_Spread.c */
int Scattering(struct FrameQueue *);
int SpreadPoints(unsigned char *, struct FrameQueue *, struct FrameQueue *);
int Spreading(unsigned char *, struct FrameQueue *, int);

/* in ColorIdentify.c */
struct HSVColor RGB2HSV(int, int, int);
struct PointMatched PointMatch(unsigned char *, int, int);

/* in FrameQueue.c */
struct FrameQueue *InitQueue();
int Enqueue(struct FrameQueue *, int);
int Dequeue(struct FrameQueue *);
int ClearQueue(struct FrameQueue *);
int QueueLength(struct FrameQueue *);

/* in ColorFileOper.c */
int RecordColor(FILE *, struct HSVColors);
int ReadColor();

#ifdef HAS_GTK
/* in GtkFunc.c */
gboolean deleted (GtkWidget *, GdkEvent *, gpointer);
gboolean socket_event (GIOChannel *, GIOCondition, gpointer);
gboolean socket_frame_event (GIOChannel *, GIOCondition, gpointer);
gboolean StartStopSearching (GtkWidget *, gpointer);
#endif

/* in Paint.c */
int PrintColor(unsigned char *, int, int);
int DrawBigPoint(unsigned char *, int, int, int);

/* in BottomLayer.c */
int get_brightness_adj (unsigned char *, long, int *);
int InitVideo ();
int RetrieveFrame (int);
int CloseVideo (int);
void *InitShared (char *);
void *OpenShared (char *);
int CloseShared (void *);
int InitMotors ();
int SendMotors (int, struct motor_step);
struct motor_step ReadMotionFile (FILE *);

/* in SocketServer.c */
int SelectClient (int);
int ServeBreakClient (int, int, int);
int ServeVisiond (int);
int BreakVisiond (int);
int ServeConsoleGuarder (int);
int BreakConsoleGuarder (int);
int ServeGtkGuarder (int);
int BreakGtkGuarder (int);
int ServeGtkGuarderFrame (int);
int BreakGtkGuarderFrame (int);
int ServeMotord (int);
int BreakMotord (int);

#endif