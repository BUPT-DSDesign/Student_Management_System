import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useTimeStore = defineStore('time', () => {
    const Tm = ref(1)
    const week = ref(1)
    const day = ref(1)
    const hour = ref(0)
    const minute = ref(0)

    const clear = () => {
        Tm.value = 1
        week.value = 1
        day.value = 1
        hour.value = 0
        minute.value = 0
    }

    const getTime = () => {
        return "第" + week.value + "周" + "-" + "星期" + day.value + "-" + (hour.value < 10 ? ("0" + hour.value) : hour.value) + ":" + (minute.value < 10 ? ("0" + minute.value) : minute.value)
    }
    
    return {
        Tm,
        week,
        day,
        hour,
        minute,
        clear,
        getTime
    }
}, {
    persist: true
})