#pragma once

/* ----- The transition functions -----
   The equations can be found on easings.net and in the GitHub repo
*/
float linear(float t);
float easeInOutCubic(float t);
float easeOutCubic(float t);
float easeInOutExponential(float t);
float easeInOutElastic(float t);
float easeOutBack(float t);
float easeInOutQuint(float t);
float easeInBack(float t);
float easeOutElastic(float t);

/// The currently supported transition functions
enum class TransitionFunction
{
    None,
    Linear,
    EaseInOutCubic,
    EaseOutCubic,
    EaseInOutExponential,
    EaseInOutElastic,
    EaseOutBack,
    EaseInBack,
    EaseOutElastic,
};

/// Calls the easing function associated with the provided enum entry
float getRatio(float t, TransitionFunction transition);