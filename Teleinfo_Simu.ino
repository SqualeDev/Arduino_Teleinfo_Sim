#include <Arduino.h>
#include <String.h>

/*
  Simulateur de compteur Teleinfo.
*/

int checksum( char* );
//int calculer_crc( char* );
String MiseEnFormeCPT(int, long);
String ChaineAZero(int, char);
char* StringToChars(String);
long IncrementeCpt();

int cptTempo = 0;
int cptDelai = 50;
long cptIndex = 630;

void setup()
{
	Serial.begin(1200);
}

void loop()
{
// Debut de la Trame
    Serial.write( 0x02 );


// Envoi (ADCO 123456789123 4)  => N° d'identification du compteur : 123456789123
    Serial.write( 0x0A );
	Serial.write( 0x41 ); Serial.write( 0x44 ); Serial.write( 0x43 ); Serial.write( 0x4F );
	Serial.write( 0x20 );
	Serial.print( "123456789123" );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("ADCO 123456789123\x20") ) );
    Serial.write( 0x0D );

// Envoi (OPTARIF BASE 0)       => Option tarifaire : BASE
    Serial.write( 0x0A );
	Serial.write( "OPTARIF" );
	Serial.write( 0x20 );
	Serial.print( "BASE" );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("OPTARIF BASE\x20") ) );
    Serial.write( 0x0D );

// Envoi (ISOUSC 45 ?)          => Intensite souscrite : 45 amperes
    Serial.write( 0x0A );
	Serial.write( "ISOUSC" );
	Serial.write( 0x20 );
	Serial.print( 45 );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("ISOUSC 45\x20") ) );
    Serial.write( 0x0D );

// Envoi (BASE 065259309 2)     => Index : 065259309
    String strCpt = MiseEnFormeCPT( 9, IncrementeCpt() );
    Serial.write( 0x0A );
	Serial.write( "BASE" );
	Serial.write( 0x20 );
	Serial.print( strCpt );
	Serial.write( 0x20 );
	//Serial.write( checksum( StringToChars( "BASE " + strCpt + ' ' ) ) );
	Serial.write( checksum( StringToChars( "BASE " + strCpt + "\x20" ) ) );
    Serial.write( 0x0D );

// Envoi (PTEC TH.. $)          => Periode tarifaire en cours : TH..
    Serial.write( 0x0A );
	Serial.write( "PTEC" );
	Serial.write( 0x20 );
	Serial.print( "TH.." );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("PTEC TH..\x20" ) ) );
    Serial.write( 0x0D );

// Envoi (IINST 003 Z)          => Intensite instantanee : 3 amperes
    Serial.write( 0x0A );
	Serial.write( "IINST" );
	Serial.write( 0x20 );
	Serial.print( "003" );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("IINST 003\x20" ) ) );
    Serial.write( 0x0D );

// Envoi (IMAX 047 J)          => Intensite maximale : 47 amperes
    Serial.write( 0x0A );
	Serial.write( "IMAX" );
	Serial.write( 0x20 );
	Serial.print( "047" );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("IMAX 047\x20" ) ) );
    Serial.write( 0x0D );

// Envoi (PAPP 00630 *)          => Puissance apparente : 630 Volt/amperes
    Serial.write( 0x0A );
	Serial.write( "PAPP" );
	Serial.write( 0x20 );
	Serial.print( "00630" );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("PAPP 00630\x20" ) ) );
    Serial.write( 0x0D );

// Envoi (MOTDETAT 000000 B)     => Mot d’etat (autocontrole) : 000000
    Serial.write( 0x0A );
	Serial.write( "MOTDETAT" );
	Serial.write( 0x20 );
	Serial.print( "000000" );
	Serial.write( 0x20 );
	Serial.write( checksum( StringToChars("MOTDETAT 000000\x20") ) );
    Serial.write( 0x0D );


// Fin de la Trame
    Serial.write( 0x03 );

    //delay(5000);
}

long IncrementeCpt(){
    if ( cptTempo > cptDelai ){
        cptTempo = 0;
        cptIndex++;
    } else {
        cptTempo++;
    }
    return cptIndex;
}

int checksum( char *strValue ){
    //unsigned char sum = 32 ;		// Somme des codes ASCII du message + un espace
	unsigned char sum = 0 ;		// Somme des codes ASCII du message + un espace
	int i ;

	for ( i = 0; i < strlen( strValue ); i++ ) sum = sum + strValue[i] ;
	sum = ( sum & 0x3F ) + 0x20 ;
	return sum;
}

//int calculer_crc(char* strValue){
//    unsigned char c, crc=0;
//    int i=0;
//    while ((c=strValue[ i ])!=0)
//    {
//        crc^=c;
//        i++;
//    }
//    crc = ( crc & 63 ) ;
//    return crc;
//}

String MiseEnFormeCPT(int intNbDigits, long intCPT){
    String strIndex = ChaineAZero( intNbDigits, '0' );

    int intDig = (int)( 1 + log10( intCPT ) );

    String strDebut = strIndex.substring( 0, ( intNbDigits - intDig ) );
    String strFull = strDebut + intCPT;
    return strFull;
}

String ChaineAZero(int intNbDigits, char charCaractere){
    String Result = "";
    int i;

    for ( i = 0; i < intNbDigits; i++ ){
        Result += charCaractere;
    }
    return Result;
}

char* StringToChars(String strChaine){
    char arrResult[strChaine.length()+1];
    strChaine.toCharArray( arrResult, strChaine.length() );
    return arrResult;
}
