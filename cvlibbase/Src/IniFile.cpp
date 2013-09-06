/*! 
 * \file IniFile.h
 * \ingroup 
 * \brief iniÁÄËÎË¾ ËÏ°Ö È¸°Ö ¼¿»õÂ×±¨ Ì¡ÂÙ ¿Ðµ¹º÷¶¦ ¼³Ì©ÂÜ.
 * \author ±®½èË§
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "iniFile.h"

static char szLF[3] = {0x0d, 0x0a, 0x00};

namespace CVLib
{

IniFile::Key::Key ()
{
}

IniFile::Key::Key (const Key& key)
{
	names.Append(key.names);
	values.Append(key.values);
	comments.Append(key.comments);
}

IniFile::Key::~Key()
{
	names.RemoveAll();
	values.RemoveAll();
	comments.RemoveAll();
}

IniFile::IniFile( SString iniPath)
{
	Path( iniPath);
	m_fCaseInsensitive = true;
}

bool IniFile::ReadFile()
{
	// Normally you would use ifstream, but the SGI CC compiler has
	// a few bugs with ifstream. So ... fstream used.
	FILE* pFile;
	char szTemp1[MAX_VALUEDATA];
	SString	szLine;
	SString   szKeyname, szValuename, szValue;

	pFile = fopen( m_szPath.GetBuffer(0), "rb");
	if ( pFile == NULL)
		return false;
 
	BOOL fSuccess = TRUE;
	while( fgets( szTemp1, MAX_VALUEDATA, pFile))
	{
		szLine =szTemp1;
		// To be compatible with Win32, check for existence of '\r'.
		// Win32 files have the '\r' and Unix files don't at the end of a szLine.
		// Note that the '\r' will be written to INI files from
		// Unix so that the created INI file can be read under Win32
		// without change.
		if ( szLine[szLine.GetLength() - 1] == '\r')
			szLine = szLine.Mid( 0, szLine.GetLength() - 1);
		else if ( szLine[szLine.GetLength() - 1] == '\n')
			szLine = szLine.Mid( 0, szLine.GetLength() - 2);
    
		if ( szLine.GetLength())
		{
			// parsing by state transition model
			int i;
			for (i = 0; i < szLine.GetLength();)
			{
				if (szLine[i] == ' ' || szLine[i] == '\t')
					i ++;
				else
					break;
			}
			szLine = szLine.Right(szLine.GetLength() - i);

			int nStrLen = szLine.GetLength();
			if (nStrLen == 0)
				goto END;
			i = 0;
			char ch;
			ch = szLine[i];
			if (ch == '[')
			{
				i ++;
				goto A;
			}
			else if (ch == '#' || ch == ';')
			{
				if ( !m_Names.GetSize())
					HeaderComment( szLine.Mid(i + 1));
				else
					KeyComment( szKeyname, szLine.Mid(i + 1));

				goto END;
			}
			else
			{
				// VËÏ±¨
				{
					SString szTemp = szLine.Mid(i);
					int nRight = szTemp.FindOneOf(" \t;#[]'\\,.<>?:\"{}=!@$%^&*()_+-=/*-+`~");
					if (nRight != -1 && 0 < nRight)
					{
						szValuename = szLine.Mid( 0, nRight);
						i += nRight;
					}
					else 
					{
						szTemp.Empty();
						goto ERR;
					}
				}
				goto D;
			}

A:
			if (i < nStrLen)
				ch = szLine[i];
			else
				goto ERR;
			if (ch == '\t' || ch == ' ')
			{
				i ++;
				goto A;
			}
			
			// KËÏ±¨
			{
				SString szTemp = szLine.Mid(i);
				int nRight = szTemp.FindOneOf(" \t;#[]'\\,.<>?:\"{}=!@$%^&*()_+-=/*-+`~");
				if (nRight != -1 && 0 < nRight)
				{
					szKeyname = szTemp.Mid(0, nRight);
					AddKeyName( szKeyname);
					i += nRight;
				}
				else
				{
					szTemp.Empty();
					goto ERR;
				}
			}
B:
			if (i < nStrLen)
				ch = szLine[i];
			else
				goto ERR;
			if (ch == '\t' || ch == ' ')
			{
				i ++;
				goto B;
			}
			else if (ch == ']')
				i ++;
C:
			if (i < nStrLen)
				ch = szLine[i];
			else
				goto END;
			if (ch == '\t' || ch == ' ')
			{
				i ++;
				goto C;
			}
			else
				goto ERR;
D:
			if (i < nStrLen)
				ch = szLine[i];
			else
				goto ERR;
			if (ch == '\t' || ch == ' ')
			{
				i ++;
				goto D;
			}
			else if (ch == '=')
				i ++;
E:
			if (i < nStrLen)
			{
				ch = szLine[i];
				if (ch == '\t' || ch == ' ')
				{
					i ++;
					goto E;
				}
			}
			else if (i > nStrLen)
				goto ERR;

			// RËÏ±¨
			{
				SString szTemp = szLine.Mid(i);
				int nRight = szTemp.FindOneOf(" \t;#");
				if (nRight != -1 && 0 < nRight)
				{
					szValue = szTemp.Mid(0, nRight);
					SetValue( szKeyname, szValuename, szValue);
					i += nRight;
				}
				else
				{
					SetValue( szKeyname, szValuename, szTemp);
					i = szLine.GetLength();
				}
			}
F:
			if (i < nStrLen)
				ch = szLine[i];
			else
				goto END;
			if (ch == '\t' || ch == ' ')
			{
				i ++;
				goto F;
			}
			else
			{
				goto ERR;
			}

ERR:
			fSuccess = FALSE;
			break;

END:
			;
		}
	}

	fclose(pFile);
	if (fSuccess == FALSE)
	{
		m_Keys.RemoveAll();
		m_Names.RemoveAll();
		m_Comments.RemoveAll();
		m_szPath.Empty();
		return false;
	}
	if ( m_Names.GetSize())
		return true;
	return false;
}

bool IniFile::WriteFile() const
{
	int nCommentID, nKeyID, nValueID;
	// Normally you would use ofstream, but the SGI CC compiler has
	// a few bugs with ofstream. So ... fstream used.
	FILE* pFile;

	pFile = fopen( m_szPath, "wb");
	if ( pFile == NULL)
		return false;

	// Write header m_Comments.
	for ( nCommentID = 0; nCommentID < m_Comments.GetSize(); ++nCommentID)
		fprintf (pFile, ";%s%s", (const char*)m_Comments[nCommentID], szLF);
	if ( m_Comments.GetSize())
		fprintf (pFile, "%s", szLF);

	// Write m_Keys and values.
	for ( nKeyID = 0; nKeyID < m_Keys.GetSize(); ++nKeyID)
	{
		fprintf (pFile, "[%s]%s", (const char*)m_Names[nKeyID], szLF);
		// Comments.
		for ( nCommentID = 0; nCommentID < ((Key*)m_Keys[nKeyID])->comments.GetSize(); ++nCommentID)
			fprintf (pFile, ";%s%s", (const char*)((Key*)m_Keys[nKeyID])->comments[nCommentID], szLF);
		// Values.
		for ( nValueID = 0; nValueID < ((Key*)m_Keys[nKeyID])->names.GetSize(); ++nValueID)
			fprintf (pFile, "%s=%s%s", (const char*)((Key*)m_Keys[nKeyID])->names[nValueID], (const char*)((Key*)m_Keys[nKeyID])->values[nValueID], szLF);
		fprintf(pFile, "%s", szLF);
	}
	fclose(pFile);

	return true;
}

long IniFile::FindKey( SString keyname) const
{
	for ( int keyID = 0; keyID < m_Names.GetSize(); ++keyID)
		if ( CheckCase( m_Names[keyID]) == CheckCase( keyname))
			return long(keyID);
	return noID;
}

long IniFile::FindValue( unsigned const keyID, SString valuename) const
{
	if ( !m_Keys.GetSize() || keyID >= (unsigned)m_Keys.GetSize())
		return noID;

	for ( int valueID = 0; valueID < ((Key*)m_Keys[keyID])->names.GetSize(); ++valueID)
	{
		if ( CheckCase( ((Key*)m_Keys[keyID])->names[valueID]) == CheckCase( valuename))
			return long(valueID);
	}
	return noID;
}

unsigned IniFile::AddKeyName( SString keyname)
{
	m_Names.Add( keyname);
	m_Keys.SetSize (m_Keys.GetSize() + 1);
	m_Keys[m_Keys.GetSize()-1]=new Key;
	return m_Names.GetSize() - 1;
}

SString IniFile::KeyName( unsigned const keyID) const
{
	if ( keyID < (unsigned)m_Names.GetSize())
		return m_Names[keyID];
	else
		return SString("");
}

unsigned IniFile::NumValues( unsigned const keyID) const
{
	if ( keyID < (unsigned)m_Keys.GetSize())
		return ((Key*)m_Keys[keyID])->names.GetSize();
	return 0;
}

unsigned IniFile::NumValues( SString keyname) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return 0;
	return ((Key*)m_Keys[keyID])->names.GetSize();
}

SString IniFile::ValueName( unsigned const keyID, unsigned const valueID) const
{
	if ( keyID < (unsigned)m_Keys.GetSize() && valueID < (unsigned)((Key*)m_Keys[keyID])->names.GetSize())
		return ((Key*)m_Keys[keyID])->names[valueID];
	return SString("");
}

SString IniFile::ValueName( SString keyname, unsigned const valueID) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return SString("");
	return ValueName( keyID, valueID);
}

bool IniFile::SetValue( unsigned const keyID, unsigned const valueID, SString szValue)
{
	if ( keyID < (unsigned)m_Keys.GetSize() && valueID < (unsigned)((Key*)m_Keys[keyID])->names.GetSize())
		((Key*)m_Keys[keyID])->values[valueID] = szValue;

	return false;
}

bool IniFile::SetValue( SString keyname, SString valuename, SString value, bool const create)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
	{
		if ( create)
			keyID = long( AddKeyName( keyname));
		else
			return false;
	}

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID) 
	{
		if ( !create)
			return false;
		((Key*)m_Keys[keyID])->names.Add( valuename);
		((Key*)m_Keys[keyID])->values.Add( value);
	} 
	else
		((Key*)m_Keys[keyID])->values[valueID] = value;

	return true;
}

bool IniFile::SetValueI( SString keyname, SString valuename, int const value, bool const create)
{
	char svalue[MAX_VALUEDATA];

	sprintf( svalue, "%d", value);
	return SetValue( keyname, valuename, SString(svalue));
}

bool IniFile::SetValueF( SString keyname, SString valuename, double const value, bool const create)
{
	char svalue[MAX_VALUEDATA];

	sprintf( svalue, "%f", value);
	return SetValue( keyname, valuename, SString(svalue));
}

bool IniFile::SetValueV( SString keyname, SString valuename, char *format, ...)
{
	va_list args;
	char value[MAX_VALUEDATA];

	va_start( args, format);
	vsprintf( value, format, args);
	va_end( args);
	return SetValue( keyname, valuename, SString(value));
}

SString IniFile::GetValue( unsigned const keyID, unsigned const valueID, SString defValue) const
{
	if ( keyID < (unsigned)m_Keys.GetSize() && valueID < (unsigned)((Key*)m_Keys[keyID])->names.GetSize())
		return ((Key*)m_Keys[keyID])->values[valueID];
	return defValue;
}

SString IniFile::GetValue( SString keyname, SString valuename, SString defValue) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return defValue;

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return defValue;

	return ((Key*)m_Keys[keyID])->values[valueID];
}

int IniFile::GetValueI(SString keyname, SString valuename, int const defValue) const
{
	char svalue[MAX_VALUEDATA];

	sprintf( svalue, "%d", defValue);
	return atoi( GetValue( keyname, valuename, SString(svalue)).GetBuffer(0)); 
}

double IniFile::GetValueF(SString keyname, SString valuename, double const defValue) const
{
	char svalue[MAX_VALUEDATA];

	sprintf( svalue, "%f", defValue);
	return atof( GetValue( keyname, valuename, SString(svalue)).GetBuffer(0)); 
}

// 16 variables may be a bit of over kill, but hey, it's only code.
unsigned IniFile::GetValueV( SString keyname, SString valuename, char *format,
			      void *v1, void *v2, void *v3, void *v4,
  			      void *v5, void *v6, void *v7, void *v8,
  			      void *v9, void *v10, void *v11, void *v12,
  			      void *v13, void *v14, void *v15, void *v16)
{
	SString   value;
	// va_list  args;
	unsigned nVals;

	SString str = GetValue( keyname, valuename);
	value = str;
	if ( !value.GetLength())
		return false;
	// Why is there not vsscanf() function. Linux man pages say that there is
	// but no compiler I've seen has it defined. Bummer!
	//
	// va_start( args, format);
	// nVals = vsscanf( value.c_str(), format, args);
	// va_end( args);

	nVals = sscanf( value.GetBuffer(0), format,
		v1, v2, v3, v4, v5, v6, v7, v8,
		v9, v10, v11, v12, v13, v14, v15, v16);

	return nVals;
}

bool IniFile::DeleteValue( SString keyname, SString valuename)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return false;

  // This looks strange, but is neccessary.
// 	vector<SString>::iterator npos = ((Key*)m_Keys[keyID])->names.begin() + valueID;
// 	vector<SString>::iterator vpos = ((Key*)m_Keys[keyID])->values.begin() + valueID;
	((Key*)m_Keys[keyID])->names.RemoveAt(valueID);
	((Key*)m_Keys[keyID])->values.RemoveAt(valueID);

  return true;
}

bool IniFile::DeleteKey( SString keyname)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	// Now hopefully this destroys the vector lists within m_Keys.
	// Looking at <vector> source, this should be the case using the destructor.
	// If not, I may have to do it explicitly. Memory leak check should tell.
	// memleak_test.cpp shows that the following not required.
	//((Key*)m_Keys[keyID])->m_Names.clear();
	//((Key*)m_Keys[keyID])->values.clear();

	m_Names.RemoveAt(keyID);
	delete (Key*)m_Keys[keyID];
	m_Keys.RemoveAt(keyID);

  return true;
}

void IniFile::Erase()
{
  // This loop not needed. The vector<> destructor seems to do
  // all the work itself. memleak_test.cpp shows this.
  //for ( unsigned i = 0; i < m_Keys.GetSize(); ++i) {
  //  m_Keys[i].m_Names.clear();
  //  m_Keys[i].values.clear();
  //}
	m_Names.RemoveAll();
	m_Comments.RemoveAll();
	for (int i=0; i<m_Keys.GetSize(); i++)
		delete ((Key*)m_Keys[i]);
	m_Keys.RemoveAll();
}

void IniFile::HeaderComment( SString szComment)
{
	m_Comments.Add(szComment);
}

SString IniFile::HeaderComment( unsigned const commentID) const
{
	if ( commentID < (unsigned)m_Comments.GetSize())
		return m_Comments[commentID];
	return SString("");
}

bool IniFile::DeleteHeaderComment( unsigned nCommentID)
{
	if ( nCommentID < (unsigned)m_Comments.GetSize())
	{
		m_Comments.RemoveAt(nCommentID);
		return true;
	}
	return false;
}

unsigned IniFile::NumKeyComments( unsigned const keyID) const
{
	if ( keyID < (unsigned)m_Keys.GetSize())
		return ((Key*)m_Keys[keyID])->comments.GetSize();
	return 0;
}

unsigned IniFile::NumKeyComments( SString keyname) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return 0;
	return ((Key*)m_Keys[keyID])->comments.GetSize();
}

bool IniFile::KeyComment( unsigned const keyID, SString comment)
{
	if ( keyID < (unsigned)m_Keys.GetSize())
	{
		((Key*)m_Keys[keyID])->comments.Add( comment);
		return true;
	}
	return false;
}

bool IniFile::KeyComment( SString keyname, SString comment)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;
	return KeyComment( unsigned(keyID), comment);
}

SString IniFile::KeyComment( unsigned const keyID, unsigned const commentID) const
{
	if ( keyID < (unsigned)m_Keys.GetSize() && commentID < (unsigned)((Key*)m_Keys[keyID])->comments.GetSize())
		return ((Key*)m_Keys[keyID])->comments[commentID];
	return SString("");
}

SString IniFile::KeyComment( SString keyname, unsigned const commentID) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return SString("");
	return KeyComment( unsigned(keyID), commentID);
}

bool IniFile::DeleteKeyComment( unsigned const keyID, unsigned const nCommentID)
{
	if ( keyID < (unsigned)m_Keys.GetSize() && nCommentID < (unsigned)((Key*)m_Keys[keyID])->comments.GetSize())
	{
		((Key*)m_Keys[keyID])->comments.RemoveAt( nCommentID);
		return true;
	}
	return false;
}

bool IniFile::DeleteKeyComment( SString keyname, unsigned const commentID)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;
	return DeleteKeyComment( unsigned(keyID), commentID);
}

bool IniFile::DeleteKeyComments( unsigned const keyID)
{
	if ( keyID < (unsigned)m_Keys.GetSize())
	{
		((Key*)m_Keys[keyID])->comments.RemoveAll();
		return true;
	}
	return false;
}

bool IniFile::DeleteKeyComments( SString keyname)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;
	return DeleteKeyComments( unsigned(keyID));
}

SString IniFile::CheckCase( SString s) const
{
	if ( m_fCaseInsensitive)
		s.MakeLower();
	return s;
}

/**
  @memo     Constructor.

  @doc      Constructor. Opens the file.
  
  @param    filename	The file to open.
    
  @return   Nothing.
  
*/
PropsReader::PropsReader( const SString &filename ) : CR(0x0a), LF(0x0d), COMMENT_CHAR('#') 
{
	fh = fopen( filename, "rb" );
}


/**
  @memo     Destructor.

  @doc      Destructor. Closes the file.
    
  @return   Nothing.
  
*/
PropsReader::~PropsReader() 
{
	if (fh) fclose(fh);
}


/**
  @memo     Increments the file pointer beyond any white space.

  @doc      Increments the file pointer beyond any white space.
    
  @return   Nothing.
  
*/
void PropsReader::SkipWhiteSpace() 
{
    int ch; 
	do { 
        ch=fgetc( fh ); 
    } while( ch==' ' || ch=='\t' || ch==CR || ch==LF );
    ungetc( ch, fh );
}


/**
  @memo     Increments the file pointer to the start of the next line.

  @doc      Increments the file pointer to the start of the next line.
    
  @return   Nothing.
  
*/
void PropsReader::SkipRestOfLine() 
{
	int ch; 
	do { 
        ch=fgetc( fh ); 
    } while( ch!=EOF && ch!=CR && ch!=LF );
    ungetc( ch, fh );

	SkipWhiteSpace();
}


/**
  @memo     Returns true if more white space is present.

  @doc      Returns true if more white space is present on the current line.
    
  @return   Nothing.
  
*/
bool PropsReader::MoreNonWhiteSpaceOnLine() 
{
    char buf[256];
	int ch, n_gets = 0; 
    bool non_white = false;

	do {
      ch=fgetc( fh ); 
      buf[n_gets++] = ch;
      if ( ch!='\t' && ch!=' ' && ch!=CR && ch!=LF && ch!=EOF) {  non_white = true; break; }
    
    } while( ch!=EOF && ch!=CR && ch!=LF );

    for(int i=0;i<n_gets;i++) ungetc( buf[--n_gets], fh );	

    return non_white;
}


/**
  @memo     Increments the file pointer beyond any comments.

  @doc      Increments the file pointer beyond any comments.
    
  @return   Nothing.
  
*/
void PropsReader::SkipComments() 
{
	int ch;
	
    ch = getc( fh );        
	if ( ch==COMMENT_CHAR ) {
			
		SkipRestOfLine();                        
        SkipComments();
	} else {
		ungetc( ch, fh );
	}
}

TokenFile::TokenFile()
{
	m_CurString.Empty(); // make string info empty
	m_Delimiter = " ,\n\t";
	strtok((char *)(const char*)m_CurString, m_Delimiter);
}

TokenFile::~TokenFile()
{
	
}

bool TokenFile::NextToken(SString& tok)
{
	char *tpos;
	bool rval;
	
	if ((tpos = strtok(NULL,m_Delimiter)) == NULL) // is there any valid data left in string ?
	{
		m_CurString.Empty();
		while ((rval = this->ReadString(m_CurString)) && m_CurString.IsEmpty()) {}
		if (!rval) return false; // we reached end of file
		
		tpos = strtok((char *)(const  char*)m_CurString,m_Delimiter);
		if (tpos == NULL) return false;
	}
	
	tok = tpos;
	return true;
}

void TokenFile::WriteString(const char* lpsz)
{
	Write (lpsz, strlen(lpsz)+1, 1);
}

char* TokenFile::ReadString(char* lpsz, int nMax)
{
	int i, ch;
	
	/* Read in single line from "stdin": */
	for( i = 0; (i < nMax-1) &&  ((ch = GetC()) != EOF) && (ch != '\n'); i++ )
		lpsz[i] = (char)ch;
	
	/* Terminate string with null character: */
	lpsz[i] = '\0';
	return lpsz;
}

bool TokenFile::ReadString(SString& rString)
{
	int nMax = 10000;
	char* lpsz = (char*)malloc(nMax);
	int i, ch;
	
	/* Read in single line from "stdin": */
	for( i = 0; (i < nMax-1) &&  ((ch = GetC()) != EOF) && (ch != '\n'); i++ )
		lpsz[i] = (char)ch;
	if (Eof())
		return false;
	/* Terminate string with null character: */
	lpsz[i] = '\0';
	rString = lpsz;
	free(lpsz);
	return true;
}

}