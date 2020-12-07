#include<QHash>
class smlUnits
{
public:
    QString *unit;
 QHash<QString,QString> smlUnits;
QString smlGetUnit(const QString &code);
};

//QString smlUnits::smlGetUnit(const QString &code)
//{
//    unit=new QString;
//    smlUnits.insert("1", "a");
//    smlUnits.insert("2", "mo");
//    smlUnits.insert("3", "wk");
//    smlUnits.insert("4", "d");
//    smlUnits.insert("5", "h");
//    smlUnits.insert("6", "min");
//    smlUnits.insert("7", "s");
//    smlUnits.insert("8", "°");
//    smlUnits.insert("9", "°C");
//    smlUnits.insert("10", "currency");
//    smlUnits.insert("11", "m");
//    smlUnits.insert("12", "m/s");
//    smlUnits.insert("13", "m³");
//    smlUnits.insert("14", "m³");
//    smlUnits.insert("15", "m³/h");
//    smlUnits.insert("16", "m³/h");
//    smlUnits.insert("17", "m³/d");
//    smlUnits.insert("18", "m³/d");
//    smlUnits.insert("19", "l");
//    smlUnits.insert("20", "kg");
//    smlUnits.insert("21", "N");
//    smlUnits.insert("22", "Nm");
//    smlUnits.insert("23", "Pa");
//    smlUnits.insert("24", "bar");
//    smlUnits.insert("25", "J");
//    smlUnits.insert("26", "J/h");
//    smlUnits.insert("27", "W");
//    smlUnits.insert("28", "VA");
//    smlUnits.insert("29", "var");
//    smlUnits.insert("30", "Wh");
//    smlUnits.insert("31", "VAh");
//    smlUnits.insert("32", "varh");
//    smlUnits.insert("33", "A");
//    smlUnits.insert("34", "C");
//    smlUnits.insert("35", "V");
//    smlUnits.insert("36", "V/m");
//    smlUnits.insert("37", "F");
//    smlUnits.insert("38", "Ω");
//    smlUnits.insert("39", "Ωm²/m");
//    smlUnits.insert("40", "Wb");
//    smlUnits.insert("41", "T");
//    smlUnits.insert("42", "A/m");
//    smlUnits.insert("43", "H");
//    smlUnits.insert("44", "Hz");
//    smlUnits.insert("45", "1/(Wh)");
//    smlUnits.insert("46", "1/(varh)");
//    smlUnits.insert("47", "1/(VAh)");
//    smlUnits.insert("48", "V²h");
//    smlUnits.insert("49", "A²h");
//    smlUnits.insert("50", "kg/s");
//    smlUnits.insert("51", "S, mho");
//    smlUnits.insert("52", "K");
//    smlUnits.insert("53", "1/(V²h)");
//    smlUnits.insert("54", "1/(A²h)");
//    smlUnits.insert("55", "1/m³");
//    smlUnits.insert("56", "%");
//    smlUnits.insert("57", "Ah");
//    smlUnits.insert("60", "Wh/m³");
//    smlUnits.insert("61", "J/m³");
//    smlUnits.insert("62", "Mol %³");
//    smlUnits.insert("63", "g/m³");
//    smlUnits.insert("64", "Pa s");
//    smlUnits.insert("253", "(reserved)");
//    smlUnits.insert("254", "(other)");
//    smlUnits.insert("255", "(unitless)");
//     smlUnits.insert("0", "");

//    return *unit;
//}
