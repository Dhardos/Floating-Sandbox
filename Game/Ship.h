/***************************************************************************************
 * Original Author:     Gabriele Giuseppini
 * Created:             2018-01-21
 * Copyright:           Gabriele Giuseppini  (https://github.com/GabrieleGiuseppini)
 ***************************************************************************************/
#pragma once

#include "GameParameters.h"
#include "MaterialDatabase.h"
#include "Physics.h"
#include "RenderContext.h"
#include "ShipDefinition.h"

#include <GameCore/GameTypes.h>
#include <GameCore/RunningAverage.h>
#include <GameCore/Vectors.h>

#include <memory>
#include <optional>
#include <vector>

namespace Physics
{

class Ship
    : public Bomb::IPhysicsHandler
{
public:

    Ship(
        ShipId id,
        World & parentWorld,
        std::shared_ptr<IGameEventHandler> gameEventHandler,
        MaterialDatabase const & materialDatabase,
        Points && points,
        Springs && springs,
        Triangles && triangles,
        ElectricalElements && electricalElements);

    ~Ship();

    ShipId GetId() const { return mId; }

    World const & GetParentWorld() const { return mParentWorld; }
    World & GetParentWorld() { return mParentWorld; }

    size_t GetPointCount() const { return mPoints.GetElementCount(); }

    auto const & GetPoints() const { return mPoints; }
    auto & GetPoints() { return mPoints; }

    auto const & GetSprings() const { return mSprings; }
    auto & GetSprings() { return mSprings; }

    auto const & GetTriangles() const { return mTriangles; }
    auto & GetTriangles() { return mTriangles; }

    auto const & GetElectricalElements() const { return mElectricalElements; }
    auto & GetElectricalElements() { return mElectricalElements; }

    void MoveBy(
        vec2f const & offset,
        GameParameters const & gameParameters);

    void RotateBy(
        float angle,
        vec2f const & center,
        GameParameters const & gameParameters);

    void DestroyAt(
        vec2f const & targetPos,
        float radiusMultiplier,
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void SawThrough(
        vec2f const & startPos,
        vec2f const & endPos,
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void DrawTo(
        vec2f const & targetPos,
        float strength,
        GameParameters const & gameParameters);

    void SwirlAt(
        vec2f const & targetPos,
        float strength,
        GameParameters const & gameParameters);

    bool TogglePinAt(
        vec2f const & targetPos,
        GameParameters const & gameParameters);

    bool InjectBubblesAt(
        vec2f const & targetPos,
        float currentSimulationTime,
        GameParameters const & gameParameters);

    bool FloodAt(
        vec2f const & targetPos,
        float waterQuantityMultiplier,
        GameParameters const & gameParameters);

    bool ToggleAntiMatterBombAt(
        vec2f const & targetPos,
        GameParameters const & gameParameters);

    bool ToggleImpactBombAt(
        vec2f const & targetPos,
        GameParameters const & gameParameters);

    bool ToggleRCBombAt(
        vec2f const & targetPos,
        GameParameters const & gameParameters);

    bool ToggleTimerBombAt(
        vec2f const & targetPos,
        GameParameters const & gameParameters);

    void DetonateRCBombs();

    void DetonateAntiMatterBombs();

    bool ScrubThrough(
        vec2f const & startPos,
        vec2f const & endPos,
        GameParameters const & gameParameters);

    ElementIndex GetNearestPointAt(
        vec2f const & targetPos,
        float radius) const;

    bool QueryNearestPointAt(
        vec2f const & targetPos,
        float radius) const;

    void Update(
        float currentSimulationTime,
        GameParameters const & gameParameters,
        Render::RenderContext const & renderContext);

    void Render(
        GameParameters const & gameParameters,
        Render::RenderContext & renderContext);

public:

    /////////////////////////////////////////////////////////////////////////
    // Dynamics
    /////////////////////////////////////////////////////////////////////////

    // Mechanical

    void UpdateMechanicalDynamics(
        float currentSimulationTime,
        GameParameters const & gameParameters,
        Render::RenderContext const & renderContext);

    void UpdatePointForces(GameParameters const & gameParameters);

    void UpdateSpringForces(GameParameters const & gameParameters);

    void IntegrateAndResetPointForces(GameParameters const & gameParameters);

    void HandleCollisionsWithSeaFloor(GameParameters const & gameParameters);

    void TrimForWorldBounds(
        float currentSimulationTime,
        GameParameters const & gameParameters);

    // Water

    void UpdateWaterDynamics(
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void UpdateWaterInflow(
        float currentSimulationTime,
        GameParameters const & gameParameters,
        float & waterTaken);

    void UpdateWaterVelocities(
        GameParameters const & gameParameters,
        float & waterSplashed);

    // Electrical

    void UpdateElectricalDynamics(
        GameWallClock::time_point currentWallclockTime,
        GameParameters const & gameParameters);

    void UpdateElectricalConnectivity(SequenceNumber currentSimulationSequenceNumber);

    void DiffuseLight(GameParameters const & gameParameters);

    // Ephemeral particles

    void UpdateEphemeralParticles(
        float currentSimulationTime,
        GameParameters const & gameParameters);

    // Misc

    void RotPoints(
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void DecaySprings(
        float currentSimulationTime,
        GameParameters const & gameParameters);

private:

    void RunConnectivityVisit();

    void DestroyConnectedTriangles(ElementIndex pointElementIndex);

    void DestroyConnectedTriangles(
        ElementIndex pointAElementIndex,
        ElementIndex pointBElementIndex);

    void PointDestroyHandler(
        ElementIndex pointElementIndex,
        bool generateDebris,
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void SpringDestroyHandler(
        ElementIndex springElementIndex,
        bool destroyAllTriangles,
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void TriangleDestroyHandler(ElementIndex triangleElementIndex);

    void ElectricalElementDestroyHandler(ElementIndex electricalElementIndex);

    void GenerateAirBubbles(
        vec2f const & position,
        float currentSimulationTime,
        PlaneId planeId,
        GameParameters const & gameParameters);

    void GenerateDebris(
        ElementIndex pointElementIndex,
        float currentSimulationTime,
        GameParameters const & gameParameters);

    void GenerateSparkles(
        ElementIndex springElementIndex,
        vec2f const & cutDirectionStartPos,
        vec2f const & cutDirectionEndPos,
        float currentSimulationTime,
        GameParameters const & gameParameters);

private:

    /////////////////////////////////////////////////////////////////////////
    // Bomb::IPhysicsHandler
    /////////////////////////////////////////////////////////////////////////

    virtual void DoBombExplosion(
        vec2f const & blastPosition,
        float sequenceProgress,
        GameParameters const & gameParameters) override;

    virtual void DoAntiMatterBombPreimplosion(
        vec2f const & centerPosition,
        float sequenceProgress,
        GameParameters const & gameParameters) override;

    virtual void DoAntiMatterBombImplosion(
        vec2f const & centerPosition,
        float sequenceProgress,
        GameParameters const & gameParameters) override;

    virtual void DoAntiMatterBombExplosion(
        vec2f const & centerPosition,
        float sequenceProgress,
        GameParameters const & gameParameters) override;

private:

#ifdef _DEBUG
    void VerifyInvariants();
#endif

private:

    ShipId const mId;
    World & mParentWorld;
    std::shared_ptr<IGameEventHandler> mGameEventHandler;
    MaterialDatabase const & mMaterialDatabase;

    // All the ship elements - never removed, the repositories maintain their own size forever
    Points mPoints;
    Springs mSprings;
    Triangles mTriangles;
    ElectricalElements mElectricalElements;

    // The current simulation sequence number
    SequenceNumber mCurrentSimulationSequenceNumber;

    // The current connectivity visit sequence number
    SequenceNumber mCurrentConnectivityVisitSequenceNumber;

    // The max plane ID we have seen - ever
    PlaneId mMaxMaxPlaneId;

    // The current electrical connectivity visit sequence number
    SequenceNumber mCurrentElectricalVisitSequenceNumber;

    // Flag remembering whether the structure of the ship (i.e. the connectivity between elements)
    // has changed since the last step.
    // When this flag is set, we'll re-detect connected components and planes, and re-upload elements
    // to the rendering context
    bool mIsStructureDirty;

    // The debug ship render mode that was in effect the last time we've uploaded elements;
    // used to detect changes and eventually re-upload
    std::optional<DebugShipRenderMode> mLastDebugShipRenderMode;

    // Initial indices of the triangles for each plane ID;
    // last extra element contains total number of triangles
    std::vector<size_t> mPlaneTrianglesRenderIndices;

    // Sinking detection
    bool mIsSinking;

    // Water
    float mTotalWater;
    RunningAverage<30> mWaterSplashedRunningAverage;

    // Pinned points
    PinnedPoints mPinnedPoints;

    // Bombs
    Bombs mBombs;

    // Force fields to apply at next iteration
    std::vector<std::unique_ptr<ForceField>> mCurrentForceFields;
};

}