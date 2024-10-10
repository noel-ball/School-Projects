"use strict"

// game config
let config = {
    parent: 'shmup',
    type: Phaser.CANVAS,
    render: {
        pixelArt: true  // prevent pixel art from getting blurred when scaled
    },
    width: 800,
    height: 600,
    scene: [shmup],
    fps: { forceSetTimeOut: true, target: 30 }
}

const game = new Phaser.Game(config);