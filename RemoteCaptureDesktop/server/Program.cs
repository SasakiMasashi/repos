using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace server
{
    [ServiceContract(Namespace = "http://WCF.CaptureDesktop")]
    public interface IService1
    {
        [OperationContract]
        byte[] getDesktopImageByteArray();

        [OperationContract]
        string getDesktopImageString();
    }

    public class Service1 : IService1
    {
        public byte[] getDesktopImageByteArray()
        {
            Bitmap bitmap = CaptureHelper.Capture();
            Console.WriteLine(DateTime.Now.ToString() + ": 画面をCaptureしました。");
            return BitmapHelper.ToByteArray(bitmap);
        }

        public string getDesktopImageString()
        {
            Bitmap bitmap = CaptureHelper.Capture();
            Console.WriteLine(DateTime.Now.ToString() + ": 画面をCaptureしました。");
            return BitmapHelper.ToString(bitmap);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            string ipAddr = NetworkUtil.GetIPAddress();
            Uri baseAddr = new Uri("http://" + ipAddr + ":8000/WCF");

            using (ServiceHost svcHost = new ServiceHost(typeof(Service1), baseAddr))
            {
                BasicHttpBinding bhBinding = new BasicHttpBinding();
                bhBinding.MaxReceivedMessageSize = 10000000;

                Console.WriteLine("サービスが立ち上がります...");
                svcHost.AddServiceEndpoint(typeof(IService1), bhBinding, "CaptureDesktop");

                svcHost.Open();

                Console.WriteLine("[Enter]キーを押すと、サービスが停止されます。");
                Console.ReadLine();

                svcHost.Close();
            }
        }
    }

    public static class CaptureHelper
    {
        public static Bitmap Capture()
        {
            Rectangle rc = Screen.PrimaryScreen.Bounds;
            Bitmap bmp = new Bitmap(rc.Width, rc.Height, PixelFormat.Format32bppPArgb);
            using (Graphics g = Graphics.FromImage(bmp))
            {
                g.CopyFromScreen(rc.X, rc.Y, 0, 0, rc.Size, CopyPixelOperation.SourceCopy);
            }

            return bmp;
        }
    }

    public static class BitmapHelper
    {
        public static byte[] ToByteArray(Bitmap bmp)
        {
            MemoryStream ms = new MemoryStream();
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            return ms.GetBuffer();
        }

        public static Bitmap ToBitMap(byte[] byteArray)
        {
            Bitmap bmp;
            using (MemoryStream ms = new MemoryStream(byteArray))
            {
                bmp = new Bitmap(ms);
            }
            return bmp;
        }

        public static Bitmap ToBitMap(string str)
        {
            byte[] byteArray = Encoding.Unicode.GetBytes(str);
            return ToBitMap(byteArray);
        }

        public static string ToString(Bitmap bmp)
        {
            MemoryStream ms = new MemoryStream();
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Jpeg);
            return Encoding.Unicode.GetString(ms.ToArray());
        }
    }

    public static class NetworkUtil
    {
        public static string GetIPAddress()
        {
            string hostName = Dns.GetHostName();
            IPHostEntry ipEntry = Dns.GetHostEntry(hostName);

            foreach (IPAddress ip in ipEntry.AddressList)
            {
                if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    return ip.ToString();
                }
            }

            return "";
        }
    }
}
