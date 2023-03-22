<!-- <template>
    <div class="time" id="time">
      <span class="hour">12</span>
      <a class="split">:</a>
      <span class="minitus">30</span>
      <a class="split">:</a>
      <span class="seconds">30</span>
    </div>
</template>
<script>
// const d = new Date()
// const h = d.getHours();
// const m = d.getMinutes();
// const s = d.getSeconds();
// time.style.setProperty('--ds', s)
// time.style.setProperty('--dm', m + s / 60)
// time.style.setProperty('--dh', h + m / 60 + s / 3600)
</script>
<style>
@property --h { 
    syntax: '<integer>';
    inherits: false;
    initial-value: 0;
}
@property --m { 
    syntax: '<integer>';
    inherits: false;
    initial-value: 0;
}
@property --s { 
    syntax: '<integer>';
    inherits: false;
    initial-value: 0;
}

html,body{
    margin: 0;
    height: 100%;
    display: grid;
    place-content: center;
}
.time{
    display: flex;
    align-items: center;
    justify-content: center;
    --step: 60s;
    font-family: Consolas, Monaco, monospace;
    font-size: 120px;
}
.split{
    animation: shark 1s step-end infinite;
}

.hour::after{
    counter-reset: hour var(--h);
    content: counter(hour, decimal-leading-zero);
    animation: hour calc(var(--step) * 60 * 24) infinite steps(24);
    animation-delay: calc( -1 * var(--step) * var(--dh) * 60);
}
.minitus::after{
    counter-reset: minitus var(--m);
    content: counter(minitus, decimal-leading-zero);
    animation: minitus calc(var(--step) * 60) infinite steps(60);
    animation-delay: calc( -1 * var(--step) * var(--dm));
}
.seconds::after{
    counter-reset: seconds var(--s);
    content: counter(seconds, decimal-leading-zero);
    animation: seconds var(--step) infinite steps(60);
    animation-delay: calc( -1 * var(--step) * var(--ds) / 60 );
}
@keyframes hour {
    to {
        --h: 24
    }
}
@keyframes minitus {
    to {
        --m: 60
    }
}
@keyframes seconds {
    to {
        --s: 60
    }
}
@keyframes shark {
    0%, 100%{
        opacity: 1;
    }
    50%{
        opacity: 0;
    }
}
</style> -->