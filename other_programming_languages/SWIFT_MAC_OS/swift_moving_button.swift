Another programming language you could use is Swift, particularly for creating a macOS application with SwiftUI. Here’s an example of how to create a similar application in Swift:

### Swift (with SwiftUI)

First, create a new macOS app project in Xcode and select SwiftUI as the user interface. Then, replace the content of `ContentView.swift` with the following code:

```swift
import SwiftUI

struct ContentView: View {
    @State private var buttonPosition = CGPoint(x: 200, y: 200)
    @State private var windowSize = CGSize(width: 800, height: 600)
    @State private var speed: CGFloat = 10
    @State private var mouseNearThreshold: CGFloat = 100

    var body: some View {
        GeometryReader { geometry in
            ZStack {
                Button(action: {
                    print("Button pressed!")
                }) {
                    Text("Click here to keep\nan idiot busy for hours!")
                        .multilineTextAlignment(.center)
                }
                .position(self.buttonPosition)
                .onAppear {
                    self.windowSize = geometry.size
                }
                .onHover { hovering in
                    if hovering {
                        self.moveButtonRandomly()
                    }
                }
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
            .background(Color.white)
            .onAppear {
                NSEvent.addLocalMonitorForEvents(matching: .mouseMoved) { event in
                    self.handleMouseMovement(event: event)
                    return event
                }
            }
        }
    }

    private func moveButtonRandomly() {
        let angle = CGFloat.random(in: 0...(2 * .pi))
        let dx = cos(angle) * speed
        let dy = sin(angle) * speed

        buttonPosition.x = min(max(0, buttonPosition.x + dx), windowSize.width - 200)
        buttonPosition.y = min(max(0, buttonPosition.y + dy), windowSize.height - 50)
    }

    private func handleMouseMovement(event: NSEvent) {
        let mouseX = event.locationInWindow.x
        let mouseY = windowSize.height - event.locationInWindow.y
        let dx = mouseX - buttonPosition.x
        let dy = mouseY - buttonPosition.y
        let dist = sqrt(dx * dx + dy * dy)

        if dist < mouseNearThreshold {
            moveButtonRandomly()
        } else {
            let centerX = windowSize.width / 2
            let centerY = windowSize.height / 2
            let distX = centerX - buttonPosition.x
            let distY = centerY - buttonPosition.y
            let centerDist = sqrt(distX * distX + distY * distY)
            if centerDist > 0 {
                buttonPosition.x += (distX / centerDist) * speed
                buttonPosition.y += (distY / centerDist) * speed
            }
        }

        buttonPosition.x = min(max(0, buttonPosition.x), windowSize.width - 200)
        buttonPosition.y = min(max(0, buttonPosition.y), windowSize.height - 50)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

@main
struct MovingButtonApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
```

This Swift code uses SwiftUI to create a macOS application with a moving button. The button moves randomly when the mouse cursor gets close to it, and it moves towards the center of the window when the cursor is far. The application listens for mouse movement events and updates the button position accordingly.