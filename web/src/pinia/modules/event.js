import { defineStore } from 'pinia'
import { EventTable } from '@/api/event'
import { ref } from 'vue'

export const useEventStore = defineStore('event', () => {
    const rdata = ref(null)
    const GetEventTable = async () => {
        try {
            const res = await EventTable()

            if (res.data.status_code == 0) {
                rdata.value = res.data
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }

    return {
        rdata,
        GetEventTable
    }
})
