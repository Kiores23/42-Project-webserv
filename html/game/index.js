function jump(event) {
    if (event.code === 'Space' && !dino.classList.contains('jumping')) {
        dino.classList.add('jumping');
        setTimeout(() => {
            dino.classList.remove('jumping');
        }, 500);
    }
}

function createObstacle() {
    const obstacle = document.createElement('div');
    obstacle.classList.add('obstacle');
    obstacle.style.left = gameContainer.clientWidth + 'px';
    gameContainer.appendChild(obstacle);

    const obstacleMove = setInterval(() => {
        const obstaclePosition = obstacle.offsetLeft;
        const dinoPosition = dino.offsetLeft;
        const dinoHeight = dino.clientHeight;
        const obstacleWidth = obstacle.clientWidth;
        const obstacleHeight = obstacle.clientHeight;

        if (
            obstaclePosition < dinoPosition + 40 &&
            obstaclePosition + obstacleWidth > dinoPosition &&
            200 - obstacleHeight < dinoHeight &&
            !dino.classList.contains('jumping') &&
            gameActive
        ) {
            gameOver();
            clearInterval(obstacleMove);
        }

        if (obstaclePosition < -obstacleWidth) {
            gameContainer.removeChild(obstacle);
            clearInterval(obstacleMove);
        }

        obstacle.style.left = obstaclePosition - 5 + 'px';
    }, 20);
}

function gameOver() {
    resetBtn.style.display = 'block'; // Afficher le bouton de relance
    while (gameContainer.firstChild) {
        gameContainer.removeChild(gameContainer.firstChild);
    }
    document.removeEventListener('keydown', jump); // Désactiver l'événement de saut
    resetBtn.addEventListener('click', resetGame);
}

function resetGame() {
    while (gameContainer.firstChild) {
        gameContainer.removeChild(gameContainer.firstChild);
    }
    createDino();
    document.addEventListener('keydown', jump); // Réactiver l'événement de saut
}

function createDino() {
    const dino = document.createElement('div');
    dino.id = 'dino';
    gameContainer.appendChild(dino);
}

function startGame() {
    playBtn.style.display = 'none';
    resetBtn.style.display = 'none';
    resetGame();
    setInterval(createObstacle, 3000);
}

const gameContainer = document.getElementById('game-container');
const playBtn = document.getElementById('play-btn');
const resetBtn = document.getElementById('reset-btn');

resetBtn.style.display = "none";
playBtn.addEventListener('click', startGame);