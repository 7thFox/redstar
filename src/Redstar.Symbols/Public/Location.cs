namespace Redstar.Symbols
{
    public struct Location
    {
        public static readonly Location Implicit = new Location(true);

        public bool IsImplicit { get; } = false;
        public string FileName { get; init; }
        public int Line { get; init; }
        public int Column { get; init; }

        private Location(bool isImplicit)
        {
            IsImplicit = isImplicit;
            FileName = "implicit";
            Line = -1;
            Column = -1;
        }

        public static implicit operator string(Location? l) => (l ?? Implicit).ToString();

        public override string ToString()
        {
            if (IsImplicit)
            {
                return "implicit";
            }
            return $"{FileName}:{Line}:{Column}";
        }
    }
}