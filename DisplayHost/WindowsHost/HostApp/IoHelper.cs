namespace HostApp
{
    internal static class IoHelper
    {
        public static string ToAbsolutePath(string path)
        {
            var result = path;
            if (path.StartsWith("."))
            {
                result = Path.Combine(Directory.GetCurrentDirectory(), path);
            }
            // collape dots
            result = Path.GetFullPath(new Uri(result).LocalPath);
            var dir = Path.GetDirectoryName(result);
            if (!Directory.Exists(dir))
            {
                throw new ApplicationException($"Directory does not exist: {dir}");
            }

            return result;
        }
    }
}
