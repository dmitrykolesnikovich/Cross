/*	Copyright © 2015 Lukyanau Maksim
 
	This file is part of Cross++ Game Engine.
 
 Cross++ Game Engine is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Cross++ is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/

#import <time.h>
#import "GLRenderView.h"
#import "Graphics.h"
#import "LauncherOS.h"
#import "InputOS.h"
#import "SnakyGame.h"
#import "Demo.h"

@implementation GLRenderView{
    LauncherOS* launcher;
    InputOS* input;
    Game* game;
}

- (id)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    
    NSLog(@"initWithCoder");
    EAGLContext* context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    self.context = context;
    self.enableSetNeedsDisplay = NO;
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    return self;
}

- (void)render:(CADisplayLink*)displayLink{
    [self display];
    if(!launcher){
        launcher = new LauncherOS();
        game = new Demo(launcher);
        Graphics* graphics = new Graphics(game);
        input = new InputOS();
        game->graphics = graphics;
        game->input = input;
        game->Start();
    }
    game->Update();
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
    input->input_state = true;
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    input->input_point.x = location.x;
    input->input_point.y = location.y;
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
    UITouch *touch = [[event allTouches] anyObject];
    CGPoint location = [touch locationInView:touch.view];
    input->input_point.x = location.x;
    input->input_point.y = location.y;
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    input->input_state = false;
}

@end
