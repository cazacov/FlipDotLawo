using System;
using System.Collections.Generic;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HostApp
{
    internal class FrameManager
    {
        public Frame LoadFrame(string fileName, int width, int height)
        {
            var result = new Frame(width, height);

            Image image = new Bitmap(fileName);

            Bitmap target = new Bitmap(width, height, PixelFormat.Format24bppRgb);

            var scaleX = (double)image.Width / width;
            var scaleY = (double)image.Height / height;

            int dx, dy;
            int sx, sy;

            if (scaleX > scaleY)
            {
                sx = width;
                sy = (int)(image.Height / scaleX);
                dx = 0;
                dy = (height - sy) / 2;
            }
            else
            {
                sx = (int)(image.Width / scaleY);
                sy = height;
                dx =  (width-sx) / 2; ;
                dy = 0;
            }

            Graphics g = Graphics.FromImage(target);
            g.InterpolationMode = InterpolationMode.Bicubic;
            g.DrawImage(
                image,
                new Rectangle(dx, dy, sx, sy),
                0, 0, image.Width, image.Height, GraphicsUnit.Pixel
            );
            image.Dispose();
            
            for (var y = 0; y < height; y++)
            {
                for (var x = 0; x < width; x++)
                {
                    var color = target.GetPixel(x, y);
                    result.Pixels[y, x] = color.GetBrightness() > 0.5;
                }
            }
            g.Dispose();
            target.Dispose();
            return result;
        }

    }
}
