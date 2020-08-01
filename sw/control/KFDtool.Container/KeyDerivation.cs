namespace KFDtool.Container
{
    public class KeyDerivation
    {
        public string DerivationAlgorithm { get; set; }

        public string HashAlgorithm { get; set; }

        public byte[] Salt { get; set; }

        public int IterationCount { get; set; }

        public int KeyLength { get; set; }
    }
}
