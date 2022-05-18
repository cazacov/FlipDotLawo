using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HostApp
{
    internal class BusPirateClient
    {
        private SerialPort port;
        public event EventHandler<ReceiveEventArgs> DataReceived;
        public bool IsConnected { get; private set; }

        public BusPirateClient()
        {
            this.port = null;
        }

        protected virtual void OnDataReceived(ReceiveEventArgs e)
        {
            var handler = DataReceived;
            handler?.Invoke(this, e);
        }

        public string[] ListPorts()
        {
            return SerialPort.GetPortNames();
        }

        public bool TryConnect(string portName)
        {
            if (this.port != null)
            {
                port.DataReceived -= PortDataReceived;
                port.Dispose();
            }

            try
            {

                this.port = new SerialPort(portName, 115200);
                this.port.Open();
                port.DataReceived += PortDataReceived;
                port.WriteLine("");
            }
            catch (Exception ex)
            {
                OnDataReceived(new ReceiveEventArgs() { Text = $"{ex.GetType().Name}: {ex.Message}" });
                IsConnected = false;
                if (port != null)
                {
                    port.Dispose();
                    port = null;
                }
                return false;
            }

            port.WriteLine("m");
            port.WriteLine("4");
            port.WriteLine("3");
            Thread.Sleep(100);
            port.WriteLine("W");
            Thread.Sleep(100);
            port.WriteLine("P");
            return true;
        }

        private void PortDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            this.IsConnected = true;
            SerialPort spL = (SerialPort)sender;
            byte[] buf = new byte[spL.BytesToRead];
            spL.Read(buf, 0, buf.Length);
            String txt = String.Empty;
            foreach (var b in buf)
            {
                char ch = (char)b;
                txt += ch;
            }
            OnDataReceived(new ReceiveEventArgs() { Text = txt});
        }

        public void SendBitmap(Frame frame, int address = 0x4E)
        {
            var builder = new StringBuilder();
            builder.Append("[ ");
            builder.Append($"{address} 9 0 0 {frame.Width} {frame.Height} ");
            var bytesInRow = (frame.Width + 7) & 0x07;

            for (var y = 0; y < frame.Height; y++)
            {
                byte nextByte = 0;
                byte mask = 0x80;
                for (var x = 0; x < frame.Width; x++)
                {
                    if (frame.Pixels[y, x])
                    {
                        nextByte |= mask;
                    }
                    mask >>= 1;
                    if (mask == 0)
                    {
                        builder.Append($"0x{nextByte:X2} ");
                        nextByte = 0;
                        mask = 0x80;
                    }
                }
                if (mask != 0x80)
                {
                    builder.Append($"0x{nextByte:X2} ");
                    nextByte = 0;
                    mask = 0x80;
                }
                if (y % 5 == 4)
                {
                    builder.Append("]");
                    var cmd = builder.ToString();
                    port.WriteLine(cmd);
                    Thread.Sleep(50);
                    builder.Clear();
                    builder.Append($"[ { address} ");
                }
            }
            builder.Append("]");
            var command = builder.ToString();
            port.WriteLine(command);
        }
    }

    public class ReceiveEventArgs : EventArgs
    {
        public string Text { get; set; }
    }
}
