// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2005 Andreas Huggel <ahuggel@gmx.net>
 * 
 * This program is part of the Exiv2 distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/*
  File:      olympusmn.cpp
  Version:   $Rev$
  Author(s): Will Stokes (wuz) <wstokes@gmail.com>
             Andreas Huggel (ahu) <ahuggel@gmx.net>
  History:   10-Mar-05, wuz: created
  Credits:   See header file.
 */

// *****************************************************************************
#include "rcsid.hpp"
EXIV2_RCSID("@(#) $Id$");

// *****************************************************************************
// included header files
#include "types.hpp"
#include "olympusmn.hpp"
#include "makernote.hpp"
#include "value.hpp"

// + standard includes
#include <string>
#include <sstream>
#include <iomanip>
#include <cassert>

// Define DEBUG_MAKERNOTE to output debug information to std::cerr
#undef DEBUG_MAKERNOTE

// *****************************************************************************
// class member definitions
namespace Exiv2 {

    //! @cond IGNORE
    OlympusMakerNote::RegisterMn::RegisterMn()
    {
        MakerNoteFactory::registerMakerNote(
            "OLYMPUS*", "*", createOlympusMakerNote); 
        MakerNoteFactory::registerMakerNote(
            olympusIfdId, MakerNote::AutoPtr(new OlympusMakerNote));

        ExifTags::registerMakerTagInfo(olympusIfdId, tagInfo_);
    }
    //! @endcond

    // Olympus Tag Info
    const TagInfo OlympusMakerNote::tagInfo_[] = {
        TagInfo(0x0200, "SpecialMode", "Picture taking mode", olympusIfdId, makerTags, unsignedLong, print0x0200),
        TagInfo(0x0201, "Quality", "Image quality setting", olympusIfdId, makerTags, unsignedShort, print0x0201),
        TagInfo(0x0202, "Macro", "Macro mode", olympusIfdId, makerTags, unsignedShort, print0x0202),
        TagInfo(0x0203, "BWMode", "Black and White Mode", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0204, "DigitalZoom", "Digital zoom ratio", olympusIfdId, makerTags, unsignedRational, print0x0204),
        TagInfo(0x0205, "FocalPlaneDiagonal", "Focal plane diagonal", olympusIfdId, makerTags, unsignedRational, printValue),
        TagInfo(0x0206, "0x0206", "Unknown", olympusIfdId, makerTags, signedShort, printValue),
        TagInfo(0x0207, "FirmwareVersion", "Software firmware version", olympusIfdId, makerTags, asciiString, printValue),
        TagInfo(0x0208, "PictureInfo", "ASCII format data such as [PictureInfo]", olympusIfdId, makerTags, asciiString, printValue),
        TagInfo(0x0209, "CameraID", "CameraID data", olympusIfdId, makerTags, undefined, printValue),
        TagInfo(0x0300, "PreCaptureFrames", "Pre-capture frames", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0301, "0x0301", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0302, "OneTouchWB", "OneTouchWB", olympusIfdId, makerTags, unsignedShort, print0x0302),
        TagInfo(0x0303, "0x0303", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0304, "0x0304", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x0f00, "DataDump", "Various camera settings", olympusIfdId, makerTags, undefined, printValue),
        TagInfo(0x1000, "0x1000", "Unknown", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1001, "0x1001", "Unknown", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1002, "0x1002", "Unknown", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1003, "0x1003", "Unknown", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1004, "FlashMode", "Flash mode", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1005, "FlashDevice", "Flash device", olympusIfdId, makerTags, unsignedShort, print0x1005),
        TagInfo(0x1006, "Bracket", "Bracket", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1007, "0x1007", "Unknown", olympusIfdId, makerTags, signedShort, printValue),
        TagInfo(0x1008, "0x1008", "Unknown", olympusIfdId, makerTags, signedShort, printValue),
        TagInfo(0x1009, "0x1009", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x100a, "0x100a", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x100b, "FocusMode", "Focus mode", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x100c, "FocusDistance", "Focus distance", olympusIfdId, makerTags, unsignedRational, printValue),
        TagInfo(0x100d, "Zoom", "Zoom", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x100e, "MacroFocus", "Macro focus", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x100f, "SharpnessFactor", "Sharpness factor", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1010, "0x1010", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1011, "ColorMatrix", "Color matrix", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1012, "BlackLevel", "Black level", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1013, "0x1013", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1014, "0x1014", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1015, "WhiteBalance", "White balance", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1016, "0x1016", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1017, "RedBalance", "Red balance", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1018, "BlueBalance", "Blue balance", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1019, "0x1019", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x101a, "SerialNumber", "Serial number", olympusIfdId, makerTags, asciiString, printValue),
        TagInfo(0x101b, "0x101b", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x101c, "0x101c", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x101d, "0x101d", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x101e, "0x101e", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x101f, "0x101f", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x1020, "0x1020", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x1021, "0x1021", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x1022, "0x1022", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x1023, "FlashBias", "Flash bias", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1024, "0x1024", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1025, "0x1025", "Unknown", olympusIfdId, makerTags, signedRational, printValue),
        TagInfo(0x1026, "0x1026", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1027, "0x1027", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1028, "0x1028", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1029, "Contrast", "Contrast setting", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x102a, "SharpnessFactor", "Sharpness factor", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x102b, "ColorControl", "Color control", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x102c, "ValidBits", "Valid bits", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x102d, "Coring Filter", "Coring filter", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x102e, "ImageWidth", "Image width", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x102f, "ImageHeight", "Image height", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x1030, "0x1030", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1031, "0x1031", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        TagInfo(0x1032, "0x1032", "Unknown", olympusIfdId, makerTags, unsignedShort, printValue),
        TagInfo(0x1033, "0x1033", "Unknown", olympusIfdId, makerTags, unsignedLong, printValue),
        // End of list marker
        TagInfo(0xffff, "(UnknownOlympusMakerNoteTag)", "Unknown OlympusMakerNote tag", olympusIfdId, makerTags, invalidTypeId, printValue)
    };

    OlympusMakerNote::OlympusMakerNote(bool alloc)
        : IfdMakerNote(olympusIfdId, alloc)
    {
        byte buf[] = {
            'O', 'L', 'Y', 'M', 'P', 0x00, 0x01, 0x00
        };
        readHeader(buf, 8, byteOrder_);
    }

    OlympusMakerNote::OlympusMakerNote(const OlympusMakerNote& rhs)
        : IfdMakerNote(rhs)
    {
    }

    int OlympusMakerNote::readHeader(const byte* buf,
                                   long len, 
                                   ByteOrder byteOrder)
    {
        if (len < 8) return 1;
  
        // Copy the header
        header_.alloc(8);
        memcpy(header_.pData_, buf, header_.size_);
        // Adjust the offset of the IFD for the prefix
        adjOffset_ = 8;

        return 0;
    }

    int OlympusMakerNote::checkHeader() const
    {
        int rc = 0;
        // Check the OLYMPUS prefix
        if (   header_.size_ < 8
            || std::string(reinterpret_cast<char*>(header_.pData_), 5) 
               != std::string("OLYMP", 5)) {
            rc = 2;
        }
        return rc;
    }

    OlympusMakerNote::AutoPtr OlympusMakerNote::create(bool alloc) const
    {
        return AutoPtr(create_(alloc));
    }

    OlympusMakerNote* OlympusMakerNote::create_(bool alloc) const
    {
        AutoPtr makerNote = AutoPtr(new OlympusMakerNote(alloc));
        assert(makerNote.get() != 0);
        makerNote->readHeader(header_.pData_, header_.size_, byteOrder_);
        return makerNote.release();
    }

    OlympusMakerNote::AutoPtr OlympusMakerNote::clone() const
    {
        return AutoPtr(clone_());
    }

    OlympusMakerNote* OlympusMakerNote::clone_() const 
    {
        return new OlympusMakerNote(*this); 
    }

    std::ostream& OlympusMakerNote::print0x0200(std::ostream& os, 
                                                const Value& value)
    {
        if (value.count() != 3 || value.typeId() != unsignedLong) {
            return os << value;
        }
        long l0 = value.toLong(0);
        switch (l0) {
        case 0: os << "Normal"; break;
        case 2: os << "Fast"; break;
        case 3: os << "Panorama"; break;
        default: os << "(" << l0 << ")"; break;
        }
        if (l0 != 0) {
            os << ", ";
            long l1 = value.toLong(1);
            os << "Sequence number " << l1;
        }
        if (l0 != 0 && l0 != 2) {
            os << ", ";
            long l2 = value.toLong(2);
            switch (l2) {
            case 1: os << "Left to Right"; break;
            case 2: os << "Right to Left"; break;
            case 3: os << "Bottom to Top"; break;
            case 4: os << "Top to Bottom"; break;
            default: os << "(" << l2 << ")"; break;
            }
        }
        return os;
    } // OlympusMakerNote::print0x0200

    //! Quality
    const TagDetails quality[] = {
        { 0, "(start)" },
        { 1, "Standard Quality (SQ)" },
        { 2, "High Quality (HQ)" },
        { 3, "Super High Quality (SHQ)" },
        { 6, "Raw" },
        { 0, "(end)" }
    };

    std::ostream& OlympusMakerNote::print0x0201(std::ostream& os, 
                                                const Value& value)
    {
        return TagTranslator(quality).print(os, value);
    } // OlympusMakerNote::print0x0201

    //! Macro
    const TagDetails macro[] = {
        { -1, "(start)" },
        {  0, "Off" },
        {  1, "On" },
        {  2, "Super Macro" },
        { -1, "(end)" }
    };

    std::ostream& OlympusMakerNote::print0x0202(std::ostream& os, 
                                                const Value& value)
    {
        return TagTranslator(macro).print(os, value);
    } // OlympusMakerNote::print0x0202

    std::ostream& OlympusMakerNote::print0x0204(std::ostream& os, 
                                                const Value& value)
    {
        float f = value.toFloat();
        if (f == 0.0 || f == 1.0) return os << "None";
        return os << std::fixed << std::setprecision(1) << f << "x";
    } // OlympusMakerNote::print0x0204

    //! OneTouchWB
    const TagDetails oneTouchWb[] = {
        { -1, "(start)" },
        {  0, "Off" },
        {  1, "On" },
        {  2, "On (Preset)" },
        { -1, "(end)" }
    };

    std::ostream& OlympusMakerNote::print0x0302(std::ostream& os, 
                                                const Value& value)
    {
        return TagTranslator(oneTouchWb).print(os, value);
    } // OlympusMakerNote::print0x0302

    //! FlashDevice
    const TagDetails flashDevice[] = {
        { -1, "(start)" },
        {  0, "None" },
        {  1, "Internal" },
        {  4, "External" },
        {  4, "Internal + External" },
        { -1, "(end)" }
    };

    std::ostream& OlympusMakerNote::print0x1005(std::ostream& os, 
                                                const Value& value)
    {
        return TagTranslator(flashDevice).print(os, value);
    } // OlympusMakerNote::print0x1005

// *****************************************************************************
// free functions

    MakerNote::AutoPtr createOlympusMakerNote(bool alloc,
                                              const byte* buf, 
                                              long len, 
                                              ByteOrder byteOrder, 
                                              long offset)
    {
        return MakerNote::AutoPtr(new OlympusMakerNote(alloc));
    }

}                                       // namespace Exiv2