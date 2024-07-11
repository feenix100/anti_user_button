Option Explicit

' enable developer tab and macros to use this
' create a form with the name MovingButtonForm
' then add a command button to the MovingButtonForm

' then create a new module, add the code below and uncomment to the new module and assign it to a button you created on the spreadsheet
' Sub ShowMovingButtonForm()
    ' create and add this to a new module
    ' assign this macro to a button on the spreadsheet
    ' enable developer tools to insert a button on the spreadsheet
   ' MovingButtonForm.Show
' End Sub


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

