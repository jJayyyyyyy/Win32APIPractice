##  [Filled Shapes](https://learn.microsoft.com/en-us/windows/win32/gdi/filled-shapes)

Filled shapes are geometric forms that are outlined by using the current pen and **filled** by using the current brush. There are five filled shapes:

*   Ellipse

*   Chord

*   Pie

*   Polygon

*   Rectangle

If you don't want to fill the shape, just use a NULL_BRUSH like below

```c
HDC hdc;
HGDIOBJ nullBrush = GetStockObject(NULL_BRUSH);
HGDIOBJ originHGDIOBJ = SelectObject(hdc, nullBrush);
Ellipse(hdc, 100, 100, 300, 200);   // The ellipse is not filled
nullBrush = SelectObject(hdc, originHGDIOBJ);
DeleteObject(nullBrush);
```

##  More References

*   [About Filled Shapes](https://learn.microsoft.com/en-us/windows/win32/gdi/about-filled-shapes)

*   [Using Filled Shapes](https://learn.microsoft.com/en-us/windows/win32/gdi/using-filled-shapes)

*   [Filled Shape Functions](https://learn.microsoft.com/en-us/windows/win32/gdi/filled-shape-functions)