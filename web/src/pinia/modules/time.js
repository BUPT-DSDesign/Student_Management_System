import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useTimeStore = defineStore('time', () => {
    const Tm = ref(1)
    const week = ref(1)
    const day = ref(1)
    const hour = ref(0)
    const minute = ref(0)
    const second = ref(0)

    const clear = () => {
        Tm.value = 1
        week.value = 1
        day.value = 1
        hour.value = 0
        minute.value = 0
        second.value = 0
    }
    
    return {
        Tm,
        week,
        day,
        hour,
        minute,
        second,
        clear
    }
}, {
    persist: true
})