#coding: utf-8

import gtk
import gobject
from numpy import arange
from math import pi

from path_spline import Path_Spline
from gettext import gettext as _
from ros_pose import Ros_Pose
from target import Targets


def draw_pixbuf(widget, event):
    pixbuf = gtk.gdk.pixbuf_new_from_file('map.png')
    widget.window.draw_pixbuf(widget.style.bg_gc[gtk.STATE_NORMAL],
                              pixbuf, 0, 0, 0, 0)


class Path_Ui(gtk.Window):

    MARKER_SIZE = 5

    def __init__(self, image_file, width, height,
                 scale=1, offset_x=0., offset_y=0., path=None):
        gtk.Window.__init__(self)
        self.set_title('Планирование маршрута')
        self.set_icon_from_file('icon.png')
        self.resize(int(width * scale), int(height * scale))
        self.connect('destroy', self.quit)

        self.pbuf = gtk.gdk.pixbuf_new_from_file(image_file)

        self.da = gtk.DrawingArea()
        self.add(self.da)
        self.da.add_events(gtk.gdk.BUTTON_PRESS_MASK)
        self.da.connect('expose-event', draw_pixbuf)
        self.da.connect('expose-event', self.do_expose)
        self.da.connect('button-press-event', self.button_pressed)
        self.show_all()

        self.scale = scale
        self.width = width
        self.height = height
        self.offset_x = offset_x
        self.offset_y = offset_y
        self.image_width = self.pbuf.get_width()
        self.image_height = self.pbuf.get_height()
        self.coordinates = {}

        self.targets = Targets()
        # self.path = Path_Spline(xp=(), yp=(), speed=0.05)
        self.path = Path_Spline(xp=(), yp=(), speed=0.5)
        gobject.timeout_add(50, self.move_loop)
        self.target_x = None
        self.target_y = None

        self.initpath = path
        self.ros_pose = Ros_Pose()

        if path is not None:
            try:
                n = len(path) / 2
                for i in range(n):
                    x = path[i * 2]
                    y = path[i * 2 + 1]
                    self.targets.add(x, y)
                    self.path.insert_point(i, (x, y))
                self.path.init_spline()
            except Exception:
                self.on_error(_('Spline init problem'))

    def move_loop(self):
        if self.path:
            self.target_x, self.target_y, _dx, _dy =\
                self.path.get_target_point(0.05)
            self.send_move_command(self.target_x, self.target_y)
        else:
            self.target_x, self.target_y = None, None
        self.da.queue_draw()
        return True

    def send_move_command(self, x, y):
        self.ros_pose.publish(x, y)

    def on_error(self, e):
        dialog = gtk.MessageDialog(self,
                                   gtk.DIALOG_DESTROY_WITH_PARENT,
                                   gtk.MESSAGE_ERROR,
                                   gtk.BUTTONS_CLOSE, e)
        dialog.run()
        dialog.destroy()

    def button_pressed(self, widget, event):

        def move_command():
            x, y = self.to_coordinate(event.x, event.y)

            targets_len = len(self.targets.targets)

            self.targets.add(x, y)
            self.path.insert_point(targets_len, (x, y))
            if targets_len >= 3:
                try:
                    self.path.init_spline()
                except SystemError:
                    self.on_error(_('Spline init problem'))
                    self.path.delete_point(targets_len + 1)
                    self.targets.drop()
                if self.path.position < 0.05:
                    self.path.position = 0.05
            else:
                self.send_move_command(x, y)
            return True

        def stop_command():
            self.targets.clear()
            self.path.clear()
            self.initpath = None

        if event.button == 1:
            move_command()
        elif event.button == 3:
            stop_command()

    def do_expose(self, widget, event):
        ctx = widget.window.cairo_create()
        ctx.set_font_size(16.0)
        self.draw_path(ctx)
        self.draw_target(ctx)
        self.draw_move_target(ctx)

    def draw_move_target(self, ctx):
        if self.target_x is None:
            return
        x, y = self.to_pixel(self.target_x, self.target_y)
        ctx.set_line_width(4)
        ctx.set_source_rgba(1.0, 0.0, 0.0, 1.0)
        ctx.set_dash([15])
        size = 15
        ctx.move_to(x - size, y - size)
        ctx.line_to(x + size, y + size)
        ctx.move_to(x - size, y + size)
        ctx.line_to(x + size, y - size)
        ctx.stroke()

    def draw_path(self, ctx):
        ctx.set_line_width(1)
        ctx.set_source_rgba(0.0, 1.0, 0.0, 1.0)
        if not self.path:
            return
        position = 0
        x, y = self.path.get_path_point(position)
        x, y = self.to_pixel(x, y)
        ctx.move_to(x, y)
        for position in arange(0, 1.01, 0.01):
            x, y = self.path.get_path_point(position)
            x, y = self.to_pixel(x, y)
            ctx.line_to(x, y)
            position += 0.01
        ctx.stroke()

    def draw_target(self, ctx):
        ctx.set_source_rgba(0.0, 1.0, 0.0, 1.0)
        for target in self.targets.targets:
            x, y = self.to_pixel(target.x, target.y)
            ctx.move_to(x, y)
            ctx.arc(x, y, Path_Ui.MARKER_SIZE, 0., 2. * pi)
            ctx.fill()
            ctx.move_to(x + Path_Ui.MARKER_SIZE, y)
            ctx.show_text('%3.2f %3.2f' % (target.x, target.y))
        ctx.stroke()

    def to_pixel(self, coord_x, coord_y):
        x = (self.height / 2 + coord_x + self.offset_x) * self.scale
        y = (self.width / 2 - coord_y - self.offset_y) * self.scale
        return (x, y)

    def to_coordinate(self, pixel_x, pixel_y):
        x = pixel_x / self.scale
        y = (self.height - pixel_y) / self.scale - 2 * self.offset_y
        return (x, y)

    def set_position(self, _id, (x, y, psi)):
        self.coordinates[_id] = (x, y, psi)
        self.da.queue_draw()
        return True

    def del_position(self, _id):
        if _id in self.coordinates.keys():
            del self.coordinates[_id]
        self.da.queue_draw()
        return True

    def run(self):
        gtk.main()

    def quit(self, _widget):
        gtk.main_quit()
