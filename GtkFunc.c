#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "MyVision.h"

extern GtkWidget *dialog, *text, *image;
int frame_id = 0, frame_inited = 0;
unsigned char *frame_map, *frame_pointer;

gboolean deleted(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	return 1;
}

gboolean socket_event(GIOChannel* iochannel, GIOCondition condition, gpointer data)
{
	char info[500], append[300];
	struct VideoInfo video_info;
//	GdkPixbuf *pixbuf;
	int i, client_id = GTK_GUARDER_ID;

	if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
	{
		return 0;
	}
	else
	{
		read (g_io_channel_unix_get_fd (iochannel), &video_info, sizeof (struct VideoInfo));
		write (g_io_channel_unix_get_fd (iochannel), &client_id, sizeof (int));

//		pixbuf = gdk_pixbuf_new_from_data ((unsigned char *)frame_map, GDK_COLORSPACE_RGB, FALSE,
//				8, CAPTURE_WIDTH, CAPTURE_HEIGHT, CAPTURE_WIDTH * 3, NULL, NULL);
//		gtk_image_set_from_pixbuf (GTK_IMAGE(image), pixbuf);

		sprintf(info, "frames per second: %d\nseconds per frame: %f", video_info.fps, video_info.spf);
		for (i = 0; i < COLOR_TYPES; i++)
		{
			sprintf(append, "\n\n%s\n\tarea: %d\n\taverage X: %d\n\taverage Y: %d",
					COLOR_NAME[i], video_info.area[i], video_info.aver_x[i], video_info.aver_y[i]);
			strcat(info, append);
		}
		gtk_label_set_text ((GtkLabel *) text, info);
		return 1;
	}
}

gboolean socket_frame_event(GIOChannel* iochannel, GIOCondition condition, gpointer data)
{
	GdkPixbuf *pixbuf;
	int client_id = GTK_GUARDER_FRAME_ID;

	if (!frame_inited)
	{
		if (!(frame_map = (unsigned char *) malloc (CAPTURE_WIDTH * CAPTURE_HEIGHT * 3)))
		{
			perror ("malloc");
			exit(-1);
		}
		frame_inited = 1;
		frame_pointer = frame_map;
	}

	if (condition & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
	{
		return 0;
	}
	else
	{
		if (frame_id == 64) /* a whole frame has been translated */
		{
			frame_id = 0;

			pixbuf = gdk_pixbuf_new_from_data ((unsigned char *) frame_map, GDK_COLORSPACE_RGB, FALSE,
					8, CAPTURE_WIDTH, CAPTURE_HEIGHT, CAPTURE_WIDTH * 3, NULL, NULL);
			gtk_image_set_from_pixbuf (GTK_IMAGE(image), pixbuf);

			frame_pointer = frame_map;
		}

		read (g_io_channel_unix_get_fd (iochannel), frame_pointer, LARGEST_DATAGRAM);
		write (g_io_channel_unix_get_fd (iochannel), &client_id, sizeof (int));

		frame_pointer += LARGEST_DATAGRAM;
		frame_id++;

		return 1;
	}
}
