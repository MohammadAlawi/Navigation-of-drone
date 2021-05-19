clear all
close all
clc
hold on

x = 1;
y = 2;
% curPosX = 80;
% curPosY = 80;
curPos = [80;80];
% lastPosX = curPosX;
% lastPosY = curPosY;
lastPos = curPos;

    subplot(3,1,1)
    plot(curPos(1),curPos(2),'or')
% 
% Xtarget = 10;
% Ytarget = 10;
target = [55;30];
pgain = 0.9;
igain = 0.01;
dgain = 0.9;
iState = [0;0];
velofactor = 1;

time = 0;
prevTime = -0.1;

targetVelo = [0;0];
velo = [0;0];
trueVelo = [0;0];
lastVelo = velo;
veloX = 0;
veloY = 0;
% veloAng = 0;
speedLimit = 5;
 
XVect = [0];
YVect = [0];

yaw = deg2rad(0); % x-axis up
pitchVect = [0];
rollVect = [0];
yawVect = [0];
veloVectX = [0];
% veloAngVect = [0];

noise = 0.;


for i = 1:1000
    deltaTime = time - prevTime;
    
%    curPosX = curPosX + (rand()-0.5)*noise + velo * cos(veloAng) * deltaTime;
%    curPosY = curPosY + (rand()-0.5)*noise + velo * sin(veloAng) * deltaTime;

    curPos(x) = curPos(x) + (rand()-0.5)*noise + velo(x) * deltaTime;
    curPos(y) = curPos(y) + (rand()-0.5)*noise + velo(y) * deltaTime;

%     if i>=10
%         yaw = yaw + deg2rad(1*i);
%     end    
%     
%     while yaw >= pi
%         yaw = yaw - 2*pi;
%     end
%     while yaw <= -pi
%         yaw = yaw + 2*pi;
%     end
    
    error = [target(x) - curPos(x); target(y) - curPos(y)];
    
    if error(x) < 0
        posNegX = -1;
    else
        posNegX = 1;
    end
    if error(y) < 0
        posNegY = -1;
    else
        posNegY = 1;
    end
    
%     targetVelo(x) = abs(error(x))^0.6 * velofactor * posNegX;
%     targetVelo(y) = abs(error(y))^0.6 * velofactor * posNegY;

    targetVelo(x) = error(x) * velofactor;
    targetVelo(y) = error(y) * velofactor;
    
    factor = abs(targetVelo(y)/targetVelo(x));

    targetSpeed = sqrt(targetVelo(x).^2 + targetVelo(y).^2);
    speedLimit = sqrt(error(x).^2 + error(y).^2)^0.6 * 0.5;

%      targetSpeed = sqrt(targetVelo(x).^2 + (factor*targetVelo(x)).^2)^0.6 * velofactor;
% 
%      
%      targetVelo(x).^2 + (factor*targetVelo(x)).^2 = (1 + factor) * targetVelo(x).^2
     
    if targetSpeed > speedLimit
        targetVelo(x) = sqrt((speedLimit).^2 / (1 + factor)) * posNegX;
        targetVelo(y) = targetVelo(x) * factor;
    end
    
%     if targetVelo(x) > 5
%         targetVelo(x) = 5;
%     elseif targetVelo(x) < -5
%         targetVelo(x) = -5;
%     end
%     
%     if targetVelo(y) > 5
%         targetVelo(y) = 5;
%     elseif targetVelo(y) < -5
%         targetVelo(y) = -5;
%     end
    
    errorVelo = targetVelo - trueVelo;
    
%    errorDist = sqrt(Xerror.^2 + Yerror.^2);
%    errorAng = atan2(Yerror,Xerror);
    
    pTerm = pgain * errorVelo;

    iState = iState + (errorVelo * deltaTime);

    % Windup guard here
    if iState(x) > 8
        iState(x) = 8;
    elseif iState(x) < -8
        iState(x) = -8;
    end
    if iState(y) > 8
        iState(y) = 8;
    elseif iState(y) < -8
        iState(y) = -8;
    end

    iTerm = igain * iState;

    dTerm = (dgain * (trueVelo-lastVelo)) / deltaTime;
    
    
    trueVeloX = (curPos(x) - lastPos(x))/deltaTime;
    trueVeloY = (curPos(y) - lastPos(y))/deltaTime;
    
    trueVelo = [trueVeloX; trueVeloY];
    
    lastPos = curPos;
    lastVelo = trueVelo;

    posCmd = pTerm + iTerm + dTerm;

%    posCmdX = posCmd * cos(errorAng);
%    posCmdY = posCmd * sin(errorAng);
    
    % pitch = x-axis
    % roll = y-axis
    % yaw = rotation around z-axis


%    desiredPitch = cos(yaw)*posCmdX + sin(yaw)*posCmdY; 
%    desiredRoll = sin(yaw)*posCmdX - cos(yaw)*posCmdY;

    desPitchRoll = [cos(yaw) sin(yaw); sin(yaw) -cos(yaw)] * posCmd;
    
    pitchCmd = desPitchRoll(1);
    rollCmd = desPitchRoll(2);
   

    if pitchCmd > 5
        rollCmd = rollCmd * 5/pitchCmd;
        pitchCmd = 5;
    elseif pitchCmd < -5
        rollCmd = rollCmd * -5/pitchCmd;
        pitchCmd = -5;
    end
    
    if rollCmd > 5
        pitchCmd = pitchCmd * 5/rollCmd;
        rollCmd = 5;
    elseif rollCmd < -5
        pitchCmd = pitchCmd * -5/rollCmd;
        rollCmd = -5;
    end

    %curPosX = curPosX + pitchCmd;
    
    %veloY = sin(yaw)*pitchCmd + cos(yaw)*rollCmd;
    %veloX = cos(yaw)*pitchCmd + sin(yaw)*rollCmd;
    
    veloX = veloX + (cos(yaw)*pitchCmd + sin(yaw)*rollCmd)*deltaTime;
    veloY = veloY + (sin(yaw)*pitchCmd - cos(yaw)*rollCmd)*deltaTime;
       
    velo = [veloX; veloY];
    %veloAng = atan2(veloY,veloX);
    
    %plot(i,velo,'.b')
    
    XVect(i) = curPos(x);
    YVect(i) = curPos(y);
    pitchVect(i) = pitchCmd;
    rollVect(i) = rollCmd;
    yawVect(i) = yaw;
%    veloVectX(i) = veloX;
%    veloVectY(i) = veloY;
    
    veloVectX(i) = trueVelo(x);
    veloVectY(i) = targetVelo(x);
    
    prevTime = time;
    time = time+0.1;
end


subplot(3,1,1)
grid on
hold on
plot(XVect,YVect,'x-b');
xlim([0 200])
ylim([0 100])
plot(target(x),target(y),'og')

subplot(3,1,2)
grid on
hold on
plot(pitchVect)
plot(rollVect)
plot(yawVect)
legend('pitch', 'roll', 'yaw')

subplot(3,1,3)
grid on
hold on
plot(veloVectX)
plot(veloVectY)
legend('velo','target')