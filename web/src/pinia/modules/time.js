import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useTimeStore = defineStore('time', () => {
    const week = ref(1) // 周
    const day = ref(1) // 星期
    const hour = ref(0) // 时
    const minute = ref(0) // 分
    const second = ref(0) // 秒

    const clear = () => {
        week.value = 1
        day.value = 1
        hour.value = 0
        minute.value = 0
        second.value = 0
    }

    const getTime = () => {
        return "第" + week.value + "周" + "-" + "星期" + day.value + "-" + (hour.value < 10 ? ("0" + hour.value) : hour.value) + ":" + (minute.value < 10 ? ("0" + minute.value) : minute.value)
    }

    const runTime = (baseTimeRun, speed) => {
        // 根据时间的和速度计算当前时间
        second.value += baseTimeRun * speed
        minute.value += Math.floor(second.value / 60)
        second.value = second.value % 60
        hour.value += Math.floor(minute.value / 60)
        minute.value = minute.value % 60
        day.value += Math.floor(hour.value / 24)
        hour.value = hour.value % 24
        week.value += Math.floor(day.value / 7)
        day.value = day.value % 7
    }

    return {
        week,
        day,
        hour,
        minute,
        second,
        clear,
        getTime,
        runTime
    }
}, {
    persist: true
})