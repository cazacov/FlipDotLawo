using System.Drawing.Drawing2D;
using System.Drawing.Imaging;

namespace HostApp
{
    internal class FrameManager
    {
        public Frame LoadFrame(string fileName, Size displaySize)
        {
            var result = new Frame(displaySize.Width, displaySize.Height);

            Image image = new Bitmap(fileName);

            Bitmap target = new Bitmap(displaySize.Width, displaySize.Height, PixelFormat.Format24bppRgb);

            var scaleX = (double)image.Width / displaySize.Width;
            var scaleY = (double)image.Height / displaySize.Height;

            int dx, dy;
            int sx, sy;

            if (scaleX > scaleY)
            {
                sx = displaySize.Width;
                sy = (int)(image.Height / scaleX);
                dx = 0;
                dy = (displaySize.Height - sy) / 2;
            }
            else
            {
                sx = (int)(image.Width / scaleY);
                sy = displaySize.Height;
                dx =  (displaySize.Width - sx) / 2; ;
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
            
            for (var y = 0; y < displaySize.Height; y++)
            {
                for (var x = 0; x < displaySize.Width; x++)
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
