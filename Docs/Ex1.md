### Prove that composing transformations in the global coordinate frame is equivalent to composing them in the local coordinate frame, but in reverse order.

Let `p` be a point represented as a column vector, and let `T_1` be invertible. Let `T_2^local` be defined in the local frame carried by `T_1`.

After applying `T_1`, the point is `T_1⋅p`. To apply the second transformation while working in global coordinates, convert `T_2^local` into a global transformation by conjugation:
```
T_2^global=T_1⋅T_2^local⋅T_1^(-1)
```
Then apply `T_2^global`:
```
p^'=T_2^global⋅(T_1⋅p)
   =(T_1⋅T_2^local⋅T_1^(-1))⋅T_1⋅p
   =T_1⋅T_2^local⋅p
```
Therefore, the two composition orders are equivalent:
```
T_2^global⋅T_1=T_1⋅T_2^local
```
Equivalently, for a transformation `T_2^global` defined in global coordinates:
```
T_2^global⋅T_1=T_1⋅(T_1^(-1)⋅T_2^global⋅T_1)
```
Here, `(T_1^(-1)⋅T_2^global⋅T_1)` is `T_2^global` pulled back into the local frame, so it equals `T_2^local`.
