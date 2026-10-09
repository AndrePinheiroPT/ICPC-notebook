# Geometry selection

The canonical layer is Jet's Point/Predicates/P2 interface, also used in H. All public vertex IDs are 0-based; geometric segments are explicitly closed sets (not array intervals). The selected implementations use vec, rep, and pb; no conflicting global P alias or arg macro.

| Input material | Team decision |
|---|---|
| H/Jet Point, Predicates, lineIntersection, LineSegInter, Projection2, PolyArea | Retained canonical APIs; concise English contracts; area2 explicitly signed and doubled. |
| Jet PointInPoly, Hull, PointInConvex, HullDiameter2, ConvexCut | Restored. Covers nonconvex membership, convex queries, diameter and clipping. |
| Jet Circle2 and EnclosingCircle | Restored. Includes circumcircle, zero-radius circles, failure states and tolerance contract. |
| Jet NearestPair and RectUnion | Restored with input IDs and rectangle [x1,x2) x [y1,y2) semantics. |
| Andre CircleTangents | Adapted to Circle/P2 and nonnegative radii; internal tangents selected by bool, no hidden negative-radius convention. |
| Andre CirclePolygonIntersection | Adapted to Circle/P2, unsigned area, either orientation, duplicate adjacent vertices, zero radius; removed global arg macro. |
| Andre PolygonCenter | Adapted to optional<P2>, reports zero area instead of dividing by zero. |
| Andre ManhattanMST | Retained candidate-edge sweep; long long weights and original IDs, explicit coordinate bounds. |
| Andre sideOf, OnSegment, SegmentIntersection, SegmentDistance, lineDistance | Replaced by Predicates, LineSegInter and Projection2; unsigned distance convention documented. |
| Andre ConvexHull, ClosestPair, InsidePolygon, PolygonArea, PolygonCut, CircleIntersection, circumcircle, MinimumEnclosingCircle | Deduplicated into the canonical corresponding headers above. |
| Andre Angle | Reserve: bespoke angular-order representation; core orientation, atan2 and rotate are already covered. |
| Andre linearTransformation | Reserve: specialised similarity transform; basic Point arithmetic remains. No claim that it cannot appear in MIUP. |

## Verification

Five existing geometry oracle batteries are migrated unchanged in mathematical assertions to tests/team/geometry-*.cpp: exact arbitrary-precision predicates, independent winding/gift-wrapping, rational clipping, circle enumeration and spatial brute force. geometry-extra.cpp adds complete-graph MST comparisons, tangent orthogonality, centroid reversal/degeneracy, and analytic disk/rectangle intersections. Current release/debug results are recorded by tests/run.py under validation/team. Historical input logs are not presented as current verification.
