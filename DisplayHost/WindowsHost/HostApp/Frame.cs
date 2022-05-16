using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HostApp
{
    internal class Frame
    {
        public readonly int Width;
        public readonly int Height;
        public bool[,] Pixels;

        public Frame(int width, int height)
        {
            this.Width = width;
            this.Height = height;
            this.Pixels = new bool[height, width];
        }

        public Bitmap Preview { get; set; }
    }
}
