class shmup extends Phaser.Scene {
    keyA;
    keyD;
    keySpace;
    score = 0;
    life = 3;

    constructor() {
        super('shmup');
        this.sprite = {sprite: {}};
    }

    preload() {
        // Assets from Kenny Assets pack "Pixel Platformer", "Cursor Pack", and "Alien UFO Pack"
        this.load.setPath('./assets/');

        // https://kenney.nl/assets/pixel-platformer
        this.load.image('char', 'tile_0003.png');

        // https://kenney.nl/assets/cursor-pack
        this.load.image('bullet', 'pointer_scifi_b.png');

        // https://kenney.nl/assets/alien-ufo-pack
        this.load.image('e1', 'shipYellow_manned.png');

        // Images from google searching
        this.load.image('stars', 'StarryBackground.jpg');
    }

    create() {
        let sp = this.sprite; // alias for readabilitys
        this.enemies = [];

        // create background
        this.bg = this.add.tileSprite(0, 0, game.config.width, game.config.height, 'stars');
        this.bg.setOrigin(0, 0);
        this.bg.setScrollFactor(0);
        
        // create player and movement
        sp.char = this.add.sprite(400, 550, 'char', 'tile_0003.png');
        sp.char.setScale(2, 2);
        this.keyA = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.A);
        this.keyD = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.D);

        // create player object
        sp.bullet = this.add.sprite(-100, -100, 'bullet', 'pointer_scifi_b.png');
        sp.bullet.setAngle(45);
        sp.bullet.visible = false;
        this.keySpace = this.input.keyboard.addKey(Phaser.Input.Keyboard.KeyCodes.SPACE); 

        // enemy type 1
        this.path1 = [
            800, 550,
            667, 400,
            533, 550,
            400, 400,
            267, 550,
            133, 400,
            0, 550,
            0, 50,
            133, 200,
            267, 50,
            400, 200,
            533, 50,
            667, 200,
            800, 50,
            800, 550,
            667, 400,
            533, 550,
            400, 400,
            267, 550,
            133, 400,
            0, 550,
            0, 50,
            133, 200,
            267, 50,
            400, 200,
            533, 50,
            667, 200,
            800, 50
        ];
        this.curve1 = new Phaser.Curves.Spline(this.path1);
        sp.enemy1 = this.add.follower(this.curve1, 10, 10, "e1");
        sp.enemy1.visible = false;
        sp.enemy1.setScale(.65, .65);
        sp.enemy1.setPosition(this.curve1.points[0].x, this.curve1.points[0].y);
        sp.enemy1.startFollow({from: .24, to: .76, delay: 0, duration: 8000, repeat: -1, rotationOffset: -45})
        sp.enemy1.visible = true;
        this.enemies.unshift(sp.enemy1);

        sp.enemy2 = this.add.follower(this.curve1, 10, 10, "e1");
        sp.enemy2.visible = false;
        sp.enemy2.setScale(.65, .65);
        sp.enemy2.setPosition(this.curve1.points[0].x, this.curve1.points[0].y);
        sp.enemy2.startFollow({from: 0.275, to: .795, delay: 0, duration: 8000, repeat: -1, rotationOffset: -45})
        sp.enemy2.visible = true;
        this.enemies.unshift(sp.enemy2);

        sp.enemy3 = this.add.follower(this.curve1, 10, 10, "e1");
        sp.enemy3.visible = false;
        sp.enemy3.setScale(.65, .65);
        sp.enemy3.setPosition(this.curve1.points[0].x, this.curve1.points[0].y);
        sp.enemy3.startFollow({from: 0.31, to: .83, delay: 0, duration: 8000, repeat: -1, rotationOffset: -45})
        sp.enemy3.visible = true;
        this.enemies.unshift(sp.enemy3);

    }

    update() {
        // alias for readability 
        let sp = this.sprite;

        // scroll background tile
        this.bg.tilePositionY -= 3;

        // value updates
        document.getElementById('score').innerHTML = this.score;
        document.getElementById('life').innerHTML = this.life;

        // player movement
        if(this.keyA.isDown) {
            if(sp.char.x >= 25) {
                sp.char.x -= 10;
                sp.char.setFlipX(false);
            }
        }
        if(this.keyD.isDown) {
            if(sp.char.x <= 775) {
                sp.char.x += 10;
                sp.char.setFlipX(true);
            }        
        }

        // player object
        if(this.keySpace.isDown && !sp.bullet.visible) {
            sp.bullet.setPosition(sp.char.x, sp.char.y);
            sp.bullet.visible = true;
        }
        if(sp.bullet.visible) {
            sp.bullet.y -= 25;
        }
        if(!(sp.bullet.x >= 0 && sp.bullet.x <= 800 && sp.bullet.y >= 0 && sp.bullet.y <= 600)) {
            sp.bullet.visible = false;
            sp.bullet.setPosition(-100, -100);
        }
        // enemy collisions
        for (let enemy of this.enemies){
            if(sp.bullet.x >= (enemy.x-40) && sp.bullet.x <= (enemy.x+40) && sp.bullet.y >= (enemy.y-40) && sp.bullet.y <= (enemy.y+40) && sp.bullet.visible == true){
                enemy.setPosition(-100, -100);
                enemy.destroy();
                this.enemies.splice(this.enemies.indexOf(enemy), 1);
                sp.bullet.setPosition(-100, -100);
                sp.bullet.visible = false;
                this.score += 100;
            }

            if(enemy.x >= (sp.char.x-40) && enemy.x <= (sp.char.x+40) && enemy.y >= (sp.char.y-40) && enemy.y <= (sp.char.y+40) ){
                enemy.setPosition(-100, -100);
                enemy.destroy();
                this.enemies.splice(this.enemies.indexOf(enemy), 1);
                this.life -= 1;
                if (this.life <= 0){
                    sp.char.setPosition(-100, -100);
                    sp.char.destroy();
                    setTimeout(() => { 
                        this.scene.restart();
                        this.score = 0;
                        this.life = 3;
                    }, 2000);
                }
            }
        }
        if(this.enemies.length <= 0){
            setTimeout(() => { 
                this.scene.restart();
                this.score = 0;
                this.life = 3;
            }, 2000);
        }
    }
}