namespace HostApp;

internal class FrameRenderer
{
    public Image RenderFrame(Frame frame, Size size)
    {
        var result = new Bitmap(size.Width, size.Height);
        var gt = Graphics.FromImage(result);
        
        gt.FillRectangle(Brushes.Black, 0, 0, size.Width, size.Height);

        for (int i = 0; i <= frame.Width; i++)
        {
            var x = (size.Width - 1) * i / frame.Width;
            gt.DrawLine(Pens.DimGray, x, 0, x, size.Height - 1);
        }
        for (int i = 0; i <= frame.Height; i++)
        {
            var y = (size.Height - 1) * i / frame.Height;
            gt.DrawLine(Pens.DimGray, 0, y, size.Width - 1, y);
        }

        var stepx = (double)(size.Width - 1) / frame.Width;
        var stepy = (double)(size.Height - 1) / frame.Height;
        var d = (int)(Math.Min(stepx, stepy) - 4);

        for (int y = 0; y < frame.Height; y++)
        {
            for (int x = 0; x < frame.Width; x++)
            {
                var xx = (int)(x * stepx + 2);
                var yy = (int)(y * stepy + 2);
                if (frame.Pixels[y, x])
                {
                    gt.FillEllipse(Brushes.Yellow, xx, yy, d, d);
                }
            }
        }

        return result;
    }
}