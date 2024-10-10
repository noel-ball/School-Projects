// Jim Whitehead
// Created: 4/14/2024
// Phaser: 3.70.0
//
// Cubey
//
// An example of putting sprites on the screen using Phaser
// 
// Art assets from Kenny Assets "Shape Characters" set:
// https://kenney.nl/assets/shape-characters

// debug with extreme prejudice
"use strict"

// game config
let game_config = {
    parent: 'phaser-game',
    type: Phaser.AUTO,
    scale: {
        mode: Phaser.Scale.FIT,
        autoCenter: Phaser.Scale.CENTER_BOTH,
        width: 240,
        height: 266,
    },
    fps: { forceSetTimeOut: true, target: 30 },
    scene: [Load, Adventure],
    render: {
        pixelArt: true,  // prevent pixel art from getting blurred when scaled
        antialias: false,
        roundPixels: true
    },
    physics: {
        default: 'arcade',
        arcade: {
            debug: false,
            gravity: {
                x: 0,
                y: 0
            }
        }
    }
}

var cursors;
const SCALE = 2.0;
var my = {sprite: {}, text: {}, 
    playerVal: {max: 6,
        health: 6,
        coins: 0,
        keys: 0,
    }
};

const game = new Phaser.Game(game_config);
