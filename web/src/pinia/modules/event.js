import { defineStore } from 'pinia'
import { EventTable } from '@/api/event'
import { ref } from 'vue'

export const useEventStore = defineStore('event', () => {
    const eventlist = ref({})
    const GetEventTable = async () => {
        try {
            const res = await EventTable()

            if (res.data.status_code == 0) {
                eventlist.value = res.data.activities
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }

    return {
        eventlist,
        GetEventTable
    }
}, {
    persist: true
})
