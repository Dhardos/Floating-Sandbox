/***************************************************************************************
* Original Author:		Gabriele Giuseppini
* Created:				2019-01-26
* Copyright:			Gabriele Giuseppini  (https://github.com/GabrieleGiuseppini)
***************************************************************************************/
#pragma once

#include "ShipMetadata.h"

#include <GameCore/ImageData.h>
#include <GameCore/Vectors.h>

#include <filesystem>
#include <memory>
#include <optional>
#include <string>

/*
* A partial ship definition, suitable for a preview of the ship.
*/
struct ShipPreview
{
public:

    RgbaImageData PreviewImage;
    ImageSize OriginalSize;
    ShipMetadata Metadata;

    static ShipPreview Load(
        std::filesystem::path const & filepath,
        ImageSize const & maxSize);

    ShipPreview(ShipPreview && other)
        : PreviewImage(std::move(other.PreviewImage))
        , OriginalSize(std::move(other.OriginalSize))
        , Metadata(std::move(other.Metadata))
    {
    }

private:

    ShipPreview(
        RgbaImageData previewImage,
        ImageSize originalSize,
        ShipMetadata metadata)
        : PreviewImage(std::move(previewImage))
        , OriginalSize(std::move(originalSize))
        , Metadata(std::move(metadata))
    {
    }
};
