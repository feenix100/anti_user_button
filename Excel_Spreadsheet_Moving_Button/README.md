Follow these instructions to create in Microsoft Excel a button that can not be clicked on!

To create a button on an Excel spreadsheet that opens a UserForm with a moving button, follow these steps:

1. **Create the UserForm:**

   - Open the VBA editor by pressing `ALT + F11`.
   - Insert a new UserForm by right-clicking on any existing module or `ThisWorkbook` in the Project Explorer, selecting `Insert`, and then `UserForm`.
   - Name the UserForm `MovingButtonForm`.

2. **Add the Moving Button to the UserForm:**

   - While in the VBA editor, select `MovingButtonForm` from the Project Explorer.
   - Add a CommandButton to the form. Name it `btnMovingButton` and set its `Caption` property to "Catch me if you can!".

3. **Add the VBA Code to Make the Button Move:**

   - Double-click on the `MovingButtonForm` in the Project Explorer to open its code window.
   - Add the following code to the `MovingButtonForm`:

```vba
Option Explicit

Private Sub UserForm_Initialize()
    ' Set initial position of the button
    btnMovingButton.Left = (Me.Width - btnMovingButton.Width) / 2
    btnMovingButton.Top = (Me.Height - btnMovingButton.Height) / 2
End Sub

Private Sub btnMovingButton_MouseMove(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal Y As Single)
    MoveButton
End Sub

Private Sub MoveButton()
    Dim maxLeft As Double, maxTop As Double

    ' Calculate maximum possible positions for the button
    maxLeft = Me.Width - btnMovingButton.Width
    maxTop = Me.Height - btnMovingButton.Height

    ' Move button to a random location within the form
    btnMovingButton.Left = Application.WorksheetFunction.RandBetween(0, maxLeft)
    btnMovingButton.Top = Application.WorksheetFunction.RandBetween(0, maxTop)
End Sub


4. **Create a Button on the Worksheet to Open the UserForm:**

   - Close the VBA editor and return to Excel.
   - Go to the `Developer` tab. If you don't see the Developer tab, enable it by going to `File -> Options -> Customize Ribbon` and checking the `Developer` checkbox.
   - Click `Insert` in the `Controls` group, then choose `Button (Form Control)`.
   - Draw the button on the worksheet and, when prompted, assign it to the following macro:


Sub ShowMovingButtonForm()
    MovingButtonForm.Show
End Sub


5. **Add the Macro to Open the UserForm:**

   - In the VBA editor, insert a new module if you don't already have one (right-click on any existing module or `ThisWorkbook` in the Project Explorer, select `Insert`, and then `Module`).
   - Add the following code to the module:


Sub ShowMovingButtonForm()
    MovingButtonForm.Show
End Sub


Now, clicking the button on your worksheet will open the `MovingButtonForm` with the moving button inside it. When you move the mouse over the button, it will relocate to a random position within the form.


## To enable macros in excel and try this button that will move out of the way before you can click on it:

Select the File tab and choose Options.

Select Trust Center, and then choose Trust Center Settings.

In the Trust Center, select Macro Settings.