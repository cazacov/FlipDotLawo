using System.Text;

namespace HostApp;

internal class FrameRenderer
{
    private StreamWriter fileWriter = null;
    private Size displaySize;
    private int matrixCount;

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
        gt.Dispose();
        return result;
    }

    public void StartCode(string outputPath, Size displaySize)
    {
        this.displaySize = displaySize;
        outputPath = Path.Combine(outputPath, $"frames{displaySize.Width}x{displaySize.Height}.h");
        if (File.Exists(outputPath))
        {
            File.Delete(outputPath);
        }

        this.matrixCount = 0;
        matrixCount = displaySize.Width * displaySize.Height / (28 * 19);

        this.fileWriter = File.CreateText(outputPath);

        fileWriter.WriteLine("#pragma once");
        fileWriter.WriteLine("# include <Arduino.h>");
        fileWriter.WriteLine();
        fileWriter.WriteLine($"const uint8_t PROGMEM frames{displaySize.Width}x{displaySize.Height}[][{matrixCount}][{76}] = {{");
        this.displaySize = displaySize;
    }

    public void StopCode()
    {
        fileWriter.WriteLine("};");
        if (fileWriter == null)
        {
            return;
        }
        fileWriter.Flush();
        fileWriter.Close();
        fileWriter.Dispose();
        fileWriter = null;
    }

    private const int bitsInByte = 7;

    public void AddCode(Frame frame, int frameNo)
    {
        var builder = new StringBuilder();
        builder.AppendLine($"\t{{ // {frameNo}");
        var bytesInRow = (frame.Width + bitsInByte - 1) / bitsInByte;
        switch (this.displaySize.Width)
        {
            case 28:
                builder.AppendLine($"\t\t{{");
                for (var y = 0; y < frame.Height; y++)
                {
                    builder.Append("\t\t\t");
                    byte nextByte = 0;
                    for (var x = 0; x < frame.Width; x++)
                    {
                        var mask = (byte)(0x01 << (x % bitsInByte));
                        if (frame.Pixels[y, x])
                        {
                            nextByte |= mask;
                        }

                        if ((x % bitsInByte) == bitsInByte - 1)
                        {
                            AddByte(builder, nextByte);
                            nextByte = 0;
                        }
                    }

                    if (frame.Width % frame.Width != 0)
                    {
                        AddByte(builder, nextByte);
                    }

                    if (y != frame.Height - 1)
                    {
                        builder.AppendLine();
                    }
                }
                builder.AppendLine($"\t\t}}");
                break;
            case 38:
                builder.AppendLine($"\t\t{{");
                for (var y = 0; y < 19; y++)
                {
                    builder.Append("\t\t\t");
                    byte nextByte = 0;
                    for (var x = 0; x < 28; x++)
                    {
                        var mask = (byte)(0x01 << (x % bitsInByte));
                        if (frame.Pixels[x, 18-y])
                        {
                            nextByte |= mask;
                        }

                        if ((x % bitsInByte) == bitsInByte - 1)
                        {
                            AddByte(builder, nextByte);
                            nextByte = 0;
                        }
                    }

                    if (frame.Width % frame.Width != 0)
                    {
                        AddByte(builder, nextByte);
                    }

                    if (y != frame.Height - 1)
                    {
                        builder.AppendLine();
                    }
                }
                builder.AppendLine($"\t\t}},");
                builder.AppendLine($"\t\t{{");
                for (var y = 0; y < 19; y++)
                {
                    builder.Append("\t\t\t");
                    byte nextByte = 0;
                    for (var x = 0; x < 28; x++)
                    {
                        var mask = (byte)(0x01 << (x % bitsInByte));
                        if (frame.Pixels[27-x, 19 + y])
                        {
                            nextByte |= mask;
                        }

                        if ((x % bitsInByte) == bitsInByte - 1)
                        {
                            AddByte(builder, nextByte);
                            nextByte = 0;
                        }
                    }
                    if (y != frame.Height - 1)
                    {
                        builder.AppendLine();
                    }
                }
                builder.AppendLine($"\t\t}},");
                break;
            case 56:
                for (var matrix = 0; matrix < 4; matrix++)
                {
                    builder.AppendLine($"\t\t{{");
                    var isInverted = matrix > 1;
                    for (var y = 0; y < 19; y++)
                    {
                        builder.Append("\t\t\t");
                        byte nextByte = 0;
                        for (var x = 0; x < 28; x++)
                        {
                            var mask = (byte)(0x01 << (x % bitsInByte));
                            var frameX = matrix*28 + x;
                            var frameY = 19 + y;

                            if (isInverted)
                            {
                                frameX = 28 * (matrix - 2) + 27 - x;
                                frameY = 18 - y;
                            }
                            if (frame.Pixels[frameY, frameX])
                            {
                                nextByte |= mask;
                            }
                            if ((x % bitsInByte) == bitsInByte - 1)
                            {
                                AddByte(builder, nextByte);
                                nextByte = 0;
                            }
                        }

                        if (frame.Width % frame.Width != 0)
                        {
                            AddByte(builder, nextByte);
                        }

                        if (y != frame.Height - 1)
                        {
                            builder.AppendLine();
                        }
                    }
                    builder.AppendLine($"\t\t}},");
                }
                break;
        }
        builder.Append("\t},");
        fileWriter.WriteLine(builder.ToString());
        fileWriter.Flush();
    }

    private void AddByte(StringBuilder builder, byte nextByte)
    {
        builder.Append("0b");
        var binStr = Convert.ToString(nextByte, 2).PadLeft(8, '0');
        builder.Append(binStr);
        builder.Append(",");
    }
}