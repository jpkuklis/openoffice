/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



package org.openoffice.xmerge.converter.xml.sxw.wordsmith;

import java.io.IOException;

import org.w3c.dom.Node;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Element;

import org.openoffice.xmerge.Document;
import org.openoffice.xmerge.ConverterCapabilities;
import org.openoffice.xmerge.converter.xml.OfficeDocument;
import org.openoffice.xmerge.converter.xml.sxw.SxwDocument;
import org.openoffice.xmerge.converter.xml.*;

/**
 *  <p>This class represents a font table in a WordSmith document.
 *  A font table is represented as follows:</p>
 *
 *  <p><blockquote>
 *   binary "3"<br>
 *   two-byte length of the table of strings which follows<br>
 *   string table (null-terminated strings) representing font names
 *  </blockquote></p>
 *
 *  @author   David Proulx
 */
class WseFontTable extends Wse {

    java.util.Vector fontNames = new java.util.Vector(10);


    /**
     *  Constructor for use when going from DOM to WordSmith.
     */
    public WseFontTable() {
    }


    /**
     *  Constructor for use when going from WordSmith to DOM.
     *
     *  @param  dataArray  <code>byte</code> array.
     *  @param  i          The index.
     */
    public WseFontTable(byte dataArray[], int i) {
        i++;
        int tableLen = ((dataArray[i] << 8) | (dataArray[i+1] & 0xFF));
        i += 2;
        while (tableLen > 0) {
            int j = 0;
            while (dataArray[i + j] != 0) j++;
            fontNames.add(new String(dataArray, i, j));
            tableLen -= (j + 1);
            i += (j + 1);
        }
    }


    /**
     *  Add a new font to the table.
     *
     *  @param  newFontName  The new font name.
     */
    public void add(String newFontName) {
        if (newFontName != null)
            fontNames.add(newFontName);
    }


    /**
     *  Return a font name from the table, or null if invalid index.
     *
     *  @param  index  The font name index.
     *
     *  @return  The font name.
     */
    public String getFontName(int index) {
        try {
            return (String)fontNames.elementAt(index);
        } catch (ArrayIndexOutOfBoundsException e) {
            return null;
        }
    }

    /**
     *  Return the index of a font name in the table, or -1 if not found.
     *
     *  @param fontName  The font name.
     *
     *  @return  The index of the font name, or -1 if not found.
     */
    public int getFontIndex(String fontName) {
        int len = fontNames.size();
        for (int i = 0; i < len; i++) {
            String name = (String) fontNames.elementAt(i);
            if (name.equals(fontName)) 
                return i;
        }
        return -1;
    }


    /**
     *  Compute the index of the first <code>byte</code> following the
     *  paragraph descriptor, assuming that
     *  <code>dataArray[startIndex]</code> is the beginning of a valid
     *  paragraph descriptor.
     *
     *  @param dataArray   <code>byte</code> array.
     *  @param startIndex  The start index.
     *
     *  @return  The index of the first <code>byte</code> following the
     *           paragraph description.
     */
    static int computeNewIndex(byte dataArray[], int startIndex) {
        startIndex++;  // Skip the leading "3"
        int tableLen = ((dataArray[startIndex] << 8) | (dataArray[startIndex+1] & 0xFF));
        tableLen &= 0xFFFF;  // eliminate problems with sign-extension
        return startIndex + tableLen + 2;
    }


    /**
     *  Return true if <code>dataArray[startIndex]</code> is the start of a
     *  valid paragraph descriptor.
     *
     *  @param  dataArray   <code>byte</code> string.
     *  @param  startIndex  Start index.
     *
     *  @return  true if <code>dataArray[startIndex]</code> is the start
     *           of a valid paragraph descriptor, false otherwise.
     */
    static boolean isValid(byte dataArray[], int startIndex) {
        try {
            if (dataArray[startIndex] != 3) 
                return false;
            int len = ((dataArray[startIndex+1] << 8)
              | (dataArray[startIndex+2] & 0xFF));
            len &= 0xFFFF;  // eliminate problems with sign-extension
            
            if (dataArray[startIndex + len + 2] != 0) 
                return false;
        } catch (ArrayIndexOutOfBoundsException e) {
            return false;
        }
        return true;
    }


    /**
     *  Return the number of bytes needed to represent this font table.
     *
     *  @return The number of bytes needed to represent this font table.
     */
    int getByteCount() {

        int length = 3;  // leading "3" plus 2 bytes for length.
        int nFonts = fontNames.size();
        for (int i = 0; i < nFonts; i++) {
            String name = (String)fontNames.elementAt(i);
            length += name.length() + 1;  // extra byte is for trailing "0"
        }
        return length;        
    }

    /**
     *  Return a <code>byte</code> array representing this font table.
     *
     *  @return  An <code>byte</code> array representing this font table.
     */
    byte[] getBytes() {
        
        int length = getByteCount();
        int nFonts = fontNames.size();
        byte b[] = new byte[length];
        b[0] = 3;
        length -= 3;
        b[1] = (byte)(length >> 8);
        b[2] = (byte)(length & 0xFF);
        int indx = 3;
        for (int i = 0; i < nFonts; i++) {
            String name = (String)fontNames.elementAt(i);
            byte bname[] = name.getBytes();
            System.arraycopy(bname, 0, b, indx, bname.length);
            indx += bname.length;
            b[indx++] = 0;
        }
        return b;
    }
}

