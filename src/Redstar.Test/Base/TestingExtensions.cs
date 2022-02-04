using Microsoft.VisualStudio.TestTools.UnitTesting;
using Redstar.Symbols;

namespace Redstar.Test;

public static class TestingExtensions
{
    public static void AssertLocation(this Location expected, Location actual)
    {
        Assert.AreEqual(expected.FileName, actual.FileName, "FileName");
        Assert.AreEqual(expected.Line, actual.Line, "Line");
        Assert.AreEqual(expected.Column, actual.Column, "Column");
    }
}