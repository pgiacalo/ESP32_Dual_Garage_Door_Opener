# ESP32 Dual Garage Door Opener

Put a 2 watt, 6.8 ohm resistor between the MP1584 (voltage step down regulator) output caps and the supercap (i.e., resistor on the line to the supercap). This limits the inrush current during capacitor charging and keeps the MP1584 output stable (hopefully). Keep the MP1584’s recommended small ceramic + electrolytic caps directly at its output so the regulator sees the expected load for stability.

For your garage-door use (0.5 s bursts, a few times/day) a simple series resistor (e.g. 6.8 Ω) is a perfectly acceptable, pragmatic solution — provided you pick the resistor value & wattage appropriately and follow a few wiring/placement precautions.

Why that’s true (concise, engineering points)

Your event duty is tiny (a few times/day), so the long charge time and the small resistor energy loss are negligible.

A resistor limits the inrush so the MP1584EN won’t see a dead short at startup and won’t go into thermal/current foldback or oscillate.

It’s the simplest, cheapest, most robust approach with very few moving parts.

What “simple resistor” means in practice (concrete recommendations)

Value: use 6.8 Ω for about 0.5 A peak limit at 3.3 V (3.3 / 6.8 ≈ 0.49 A). If you want ~1 A limit use ≈3.3 Ω.

Power rating: choose 1 W minimum for 6.8 Ω (2 W preferred for margin). Initial pulse power is ~1.6 W but it decays quickly; average over the ~50 s charge is small. A 2 W metal-oxide (or 2–3 W wirewound) part is nicely conservative.

Type: metal-oxide or low-inductance wirewound power resistor. Keep it physically separated from heat-sensitive parts and give it a bit of copper or an open area to dissipate heat during charge.

Placement: put the resistor between the MP1584 output caps and the supercap (i.e., resistor on the line to the supercap). Keep the MP1584’s recommended small ceramic + electrolytic caps directly at its output so the regulator sees the expected load for stability.

Optional but recommended protections:

Bypass PFET across the resistor if you want to eliminate steady-state power loss. For a garage-door controller (few events/day) this is optional.

Polyfuse/slow-blow fuse in series with the cap path to protect traces if someone shorts the cap.

0.1 µF ceramic + 10–47 µF electrolytic at the MCU/regulator output for HF decoupling.

If you expect cold operation, prefer a resistor type whose resistance doesn’t fall too much with temperature (wirewound or metal-oxide OK).

Things to verify on the bench (do this once)

With cap discharged, apply VIN and scope the MP1584 output and the capacitor node—confirm the inrush is limited and the regulator doesn’t hiccup or oscillate.

Measure resistor temperature during/after a charge cycle (10–20 s of elevated temp is expected).

Verify that during your 0.5 s load event the 3.3 V rail stays above 3.0 V with the expected sag (should be ≈3.13 V as previously calculated).

Optional: run several cycles to check thermal behavior.

When resistor-alone is not enough

If you need very fast charge (seconds or less) or you can’t tolerate the resistor’s waste heat or time constant, use an active soft-start / PFET bypass.

If your product must have near-zero standby loss and rapid readiness, resistor + PFET bypass or an active charger is better.

Bottom line: for your stated garage-door profile, a 6.8 Ω, 2 W resistor between MP1584 output caps and the supercap, plus the usual HF decoupling and a polyfuse for protection, is a simple, reliable solution. If you want, I’ll give an exact tiny schematic showing component placement and the optional PFET bypass wiring.